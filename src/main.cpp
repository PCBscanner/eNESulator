#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../src/cleanup.h"
#include "../src/6502.h"
#include "../src/apu.h"
#include "../src/filter.h"
#include "../src/ppu.h"
#include "../src/bus.h"
#include "../src/GUI.h"

int main(int argc, char** args)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    if (TTF_Init() != 0){
        logSDLError(std::cout, "TTF_Init");
        return 1;
    }

    TTF_Font* Font;

    Font = TTF_OpenFont("../data/fonts/Swansea-q3pd.ttf", 24);

    if ( !Font ) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Window* Window = SDL_CreateWindow("eNESulator", 1000, 1000, WINDOW_WIDTH,
                                          WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowResizable(Window, SDL_TRUE);
    if (Window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (Renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(Window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* ScreenTexture = SDL_CreateTexture( Renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Texture* PatternTableTexture = SDL_CreateTexture( Renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, CHRTABLE_WIDTH, CHRTABLE_HEIGHT);
    if (ScreenTexture == nullptr || PatternTableTexture == nullptr){
        cleanup(ScreenTexture, PatternTableTexture, Renderer, Window);
        SDL_Quit();
        return 1;
    }

    APU apu;
    Bus bus;
    CPU cpu;
    PPU ppu;
    FIRFilter filter;

    //audio settings
    SDL_AudioSpec AudioSettings        = {0};
    SDL_AudioSpec AudioSettings_Actual = {0};

    AudioSettings.freq      = apu.SamplingRate;       //sampling frequency
    AudioSettings.format    = AUDIO_F32;   //16-bit sample
    AudioSettings.channels  = apu.Channels;           //nr of channels for our audio device. 1 = mono, 2 = stereo, etc
    AudioSettings.callback  = NULL;        //no callback. we'll use a queue

    std::uint32_t BytesPerSample = sizeof(float) * AudioSettings.channels; //nr of bytes per stereo sample (hence * 2)
    std::uint32_t NSamplesBuffer = 1024; //number of samples per buffer
    std::uint32_t NBytesBuffer   = NSamplesBuffer * BytesPerSample;

    //malloc returns void*: The malloc function allocates memory and returns a generic pointer of type void*.
    //I've read that malloc should be avoided in C++, and to use new/free instead, but it seems that SDL uses malloc...
    void*  SoundBuffer  = malloc(NBytesBuffer); //pointer to the memory addr where SoundBuffer shall be stored
    float* SampleOut    = (float*)SoundBuffer; //we must then cast a type to it
    float  FilterOutput = 0;

    SDL_OpenAudio(&AudioSettings, &AudioSettings_Actual);
    SDL_PauseAudio(0); //must un-pause audio first

    //Requesting user input for the ROM, and checking if it exists.
    std::string ROM;
    bool FileExists = false;
    while(!FileExists)
    {
        std::cout << "Enter the full path to the ROM: ";
        std::cin >> ROM;
        if( std::filesystem::exists(ROM) )
        {
            FileExists = true;
        }
        else
        {
            std::cout << "ROM not found. Please try again." << std::endl;
        }
    }

    bus.ConnectAPU(apu);
    bus.ConnectCPU(cpu);
    bus.ConnectPPU(ppu);
    apu.ConnectBus(bus);
    cpu.ConnectBus(bus);

    bus.LoadCartridge(ROM);

    std::vector<std::uint8_t> FramebufferPatternTables = ppu.PushPatternTablesToGUI(bus);

    std::uint32_t PPUCycles = 0;
    std::uint32_t CPUCycles = 0;
    std::uint32_t APUCycles = 0;
    float APUFramesSinceLastSample   = 0;
    float NrAPUCyclesPerSample       = (0.5* 1789773/AudioSettings.freq); //nr of APU frames between each sample.
    std::uint16_t AudioSampleCounter = 0;
    std::uint32_t NCyclesPPUWarmUp   = 29658;

    CPUCycles = cpu.Reset();
    ppu.Reset();

    //Initial warm-up of the CPU
    while(CPUCycles < NCyclesPPUWarmUp)
    {
        cpu.Execute(CPUCycles, bus);
    }
    CPUCycles = 0;

    bool NMI = false;

    //used to cap framerate
    std::uint32_t NFrames = 0;
    const float Frametime = 1000/60;
    std::uint32_t  t1     = SDL_GetTicks();
    std::uint32_t  t2     = SDL_GetTicks();

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            switch(e.type)
            {
                case SDL_QUIT:
                {
                    quit = true;
                }break;
                case SDL_KEYDOWN:
                {
                    if(e.key.keysym.sym == SDLK_r)
                    {
                        printf("Resetting the system...\n");
                        bus.Reset(ROM);
                    }
                }break;
            }
        }

        // std::vector<std::string> CPUStatusStr = {"CPU Status:", "PC: " + std::format("{:#06x}", cpu.PC), "A: " + std::format("{:#04x}", cpu.A), "X: " + std::format("{:#04x}", cpu.X), "Y: " + std::format("{:#04x}", cpu.Y)};

        while (!ppu.FrameComplete)
        {
            if(ppu.OAMDMA)
            {
                if(bus.DMACycles == 0)
                {
                    //halt cycle
                }
                else
                {
                    switch( cpu.PutCycle )
                    {
                        case(false):
                        {
                            cpu.DMAValue = bus.CPUMemory[ ( ppu.OAMDMA * 0x100 ) + bus.OAMCount ];
                            cpu.PutCycle = true;
                        }break;
                        case(true):
                        {
                            bus.OAM[bus.OAMCount] = cpu.DMAValue;
                            bus.OAMCount++;
                            cpu.PutCycle = false;
                        }break;
                    }
                }
                bus.DMACycles++;
                CPUCycles++;

                if(bus.DMACycles == 513)
                {
                    ppu.OAMDMA    = 0;
                    bus.DMACycles = 0;
                    bus.OAMCount  = 0;
                }
            }
            else
            {
                if (ppu.SetNMI(bus) && !NMI)
                {
                    cpu.ExecuteNMI(CPUCycles, bus);
                    NMI = true;
                }
                else if (!ppu.SetNMI(bus) && NMI)
                {
                    cpu.Execute(CPUCycles, bus);
                    NMI = false;
                }
                else
                {
                    cpu.Execute(CPUCycles, bus);
                }
            }
            //clocking PPU
            while (PPUCycles <= (3*CPUCycles))
            {
                ppu.Clock(bus);
                PPUCycles++;
                bus.UpdateMMIO();
            }
            //clocking APU
            while (APUCycles <= (0.5*CPUCycles))
            {
                apu.Clock();
                APUCycles++;
                FilterOutput = filter.Update(apu.Mixer()); //updating the filter every sample
                APUFramesSinceLastSample += 1.0f;
            }
            //sampling audio
            if(APUFramesSinceLastSample >= NrAPUCyclesPerSample)
            {
                //note that by this stage, we have already downsampled with the filter
                SampleOut[(2*AudioSampleCounter) + 0] = FilterOutput; //left channel
                SampleOut[(2*AudioSampleCounter) + 1] = FilterOutput; //right channel
                AudioSampleCounter++;
                if(AudioSampleCounter == NSamplesBuffer) //checking if the buffer is full
                {
                    SDL_QueueAudio(1, SoundBuffer, NBytesBuffer); //send full buffer to audio queue
                    AudioSampleCounter = 0; //reset counter
                }
                APUFramesSinceLastSample -= NrAPUCyclesPerSample; //resetting the timer
            }
        }

        NFrames++;

        ppu.FrameComplete = false;

        SDL_RenderClear(Renderer); //clearing the renderer after each frame. required to avoid drawings persisting between frames.

        SDL_SetRenderDrawColor( Renderer, 0x0, 0x00, 0x00, 0x00 ); //setting a background colour

        t2 = SDL_GetTicks();
        //capping framerate
        if( ( t2 - t1 ) < Frametime )
        {
            SDL_Delay( Frametime - (t2 - t1) );
        }

        RenderScreen(Renderer, ScreenTexture, ppu.Framebuffer);

        SDL_RenderPresent(Renderer);

        t1 = SDL_GetTicks();

    }
    TTF_Quit();
    SDL_Quit();

}

#include <cstdint>
#include <format>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../src/cleanup.h"
#include "../src/6502.h"
#include "../src/apu.h"
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
    void* SoundBuffer = malloc(NBytesBuffer); //pointer to the memory addr where SoundBuffer shall be stored
    float* SampleOut = (float*)SoundBuffer; //we must then cast a type to it
    float  SampleFilter = 0;

    SDL_OpenAudio(&AudioSettings, &AudioSettings_Actual);
    SDL_PauseAudio(0); //must un-pause audio first

    //GAMES
    // std::string ROM = "../data/roms/1942.nes";
    // std::string ROM = "../data/roms/castlevania.nes";
    // std::string ROM = "../data/roms/dk.nes";
    // std::string ROM = "../data/roms/excitebike.nes";
    // std::string ROM = "../data/roms/f1_race.nes";
    // std::string ROM = "../data/roms/ice_climber.nes";
    // std::string ROM = "../data/roms/mario_bros.nes";
    // std::string ROM = "../data/roms/popeye.nes";
    std::string ROM = "../data/roms/smb.nes";

    //UTILITIES
    // std::string ROM = "../data/test_roms/controller.nes";
    // std::string ROM = "../data/test_roms/nestest.nes";
    // std::string ROM = "../data/test_roms/palette_ram.nes";
    // std::string ROM = "../data/test_roms/sprite_ram.nes";
    // std::string ROM = "../data/test_roms/vram_access.nes";

    //AUDIO UTILITIES
    // std::string ROM = "../data/test_roms/triangle.nes";


    bus.ConnectAPU(apu);

    bus.LoadCartridge(ROM);

    std::vector<std::uint8_t> FramebufferPatternTables = ppu.PushPatternTablesToGUI(bus);

    std::uint32_t PPUCycles = 0;
    std::uint32_t CPUCycles = 0;
    std::uint32_t APUCycles = 0;
    std::uint8_t  AudioSampleTimer    = 0;
    std::uint32_t AudioSampleTimermax = (0.5* 1789773/AudioSettings.freq) + 1; //nr of APU frames between each sample. roughly 44.1 kHz
    std::uint16_t AudioSampleCounter  = 0;
    std::uint32_t NCyclesPPUWarmUp    = 29658;

    CPUCycles = cpu.Reset(bus, ppu);

    //Initial warm-up of the CPU
    while(CPUCycles < NCyclesPPUWarmUp)
    {
        cpu.Execute(CPUCycles, bus, ppu);
    }
    CPUCycles = 0;


    bool NMI = false;

    //used to cap framerate
    std::uint32_t NFrames = 0;
    float Frametime = 16.67*1;
    std::uint32_t  t1     = SDL_GetTicks();
    std::uint32_t  t2     = SDL_GetTicks();

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT){
                quit = true;
            }
        }

        // std::vector<std::string> CPUStatusStr = {"CPU Status:", "PC: " + std::format("{:#06x}", cpu.PC), "A: " + std::format("{:#04x}", cpu.A), "X: " + std::format("{:#04x}", cpu.X), "Y: " + std::format("{:#04x}", cpu.Y)};

        while (!ppu.FrameComplete)
        {
            if(ppu.OAMDMA)
            {
                switch( bus.DMACycles % 2 )
                {
                    case(0):
                    {
                        cpu.DMAValue = bus.CPUMemory[ ( ppu.OAMDMA * 0x100 ) + ( bus.DMACycles >> 1 ) ];
                    }break;
                    case(1):
                    {
                        bus.OAM[(bus.DMACycles - 1) >> 1] = cpu.DMAValue;
                    }break;
                }
                bus.DMACycles++;
                CPUCycles++;

                if(bus.DMACycles == 512)
                {
                    ppu.OAMDMA    = 0;
                    bus.DMACycles = 0;
                }
            }
            else
            {
                if (ppu.SetNMI(bus) && !NMI)
                {
                    cpu.ExecuteNMI(CPUCycles, bus, ppu);
                    NMI = true;
                }
                else if (!ppu.SetNMI(bus) && NMI)
                {
                    cpu.Execute(CPUCycles, bus, ppu);
                    NMI = false;
                }
                else
                {
                    cpu.Execute(CPUCycles, bus, ppu);
                }
            }
            //clocking PPU
            while (PPUCycles <= (3*CPUCycles))
            {
                ppu.Clock(bus);
                PPUCycles++;
                bus.UpdateMMIO(ppu);
            }
            //clocking APU
            while (APUCycles <= (0.5*CPUCycles))
            {
                apu.Clock(bus);
                APUCycles++;
                AudioSampleTimer++;
                SampleFilter+=apu.Mixer();
            }
            //sampling audio
            if(AudioSampleTimer >= AudioSampleTimermax)
            {
                SampleOut[(2*AudioSampleCounter) + 0] = SampleFilter/AudioSampleTimer; //left channel
                SampleOut[(2*AudioSampleCounter) + 1] = SampleFilter/AudioSampleTimer; //right channel
                // SampleOut[(2*AudioSampleCounter) + 0] = apu.Mixer(); //left channel
                // SampleOut[(2*AudioSampleCounter) + 1] = apu.Mixer(); //right channel
                AudioSampleCounter++;
                if(AudioSampleCounter == (NSamplesBuffer - 1))
                {
                    SDL_QueueAudio(1, SoundBuffer, NBytesBuffer); //send buffer to audio queue
                    AudioSampleCounter = 0; //reset counter
                }
                SampleFilter     = 0;
                AudioSampleTimer = 0; //resetting the timer
            }
        }

        NFrames++;

        ppu.FrameComplete = false;

        SDL_RenderClear(Renderer); //clearing the renderer after each frame. required to avoid drawings persisting between frames.

        SDL_SetRenderDrawColor( Renderer, 0x0, 0x00, 0x00, 0x00 ); //setting a background colour

        t2 = SDL_GetTicks();

        //capping framerate to 60 fps.
        if( ( t2 - t1 ) < Frametime )
        {
            SDL_Delay( Frametime - (t2 - t1) );
        }

        // RenderPatternTables(Renderer, PatternTableTexture, FramebufferPatternTables);

        RenderScreen(Renderer, ScreenTexture, ppu.Framebuffer);

        // RenderCPUStatus(Renderer, Font, CPUStatusStr);

        SDL_RenderPresent(Renderer);

        t1 = SDL_GetTicks();

    }
    TTF_Quit();
    SDL_Quit();

}

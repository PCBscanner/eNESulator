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

struct FIRFilter
{
    static const std::uint16_t FilterLength = 120;
    float Buffer[FilterLength] = {0};
    std::uint16_t BufferIndex = 0;
    float Output = 0;

    //http://t-filter.engineerjs.com/
    static constexpr float FIRImpulseResponse[FilterLength] = {
        0.000019221241265146985,
        -0.00011693312320218828,
        -0.00005145857598133435,
        0.00008581389247283942,
        -0.00012780301900775913,
        0.00010668051341248389,
        -0.000008622727505666933,
        -0.00014028199222028285,
        0.0002685338232764779,
        -0.0002890473752543451,
        0.00014754457642863944,
        0.0001334110847936313,
        -0.00044126984894593514,
        0.0006095914860387138,
        -0.0004971931160782956,
        0.00007621217304807374,
        0.0005184131163431792,
        -0.001016179206714934,
        0.0011211539754743315,
        -0.0006662806500969817,
        -0.0002619526215376982,
        0.0013037060562800817,
        -0.001946857468262416,
        0.001758397683263153,
        -0.0006298476311485767,
        -0.0010894932717156326,
        0.002666628896147836,
        -0.0032733721006291735,
        0.0023868140851515977,
        -0.00012046829336579358,
        -0.002697027834370035,
        0.004787202334285269,
        -0.0049665175529753905,
        0.0027530718998398465,
        0.0012548494659758206,
        -0.005459767344046894,
        0.00785257325051926,
        -0.0069131311342361754,
        0.0024426666581136907,
        0.004087987482699095,
        -0.009930050039884208,
        0.012133969863481234,
        -0.00892158194730952,
        0.0007562479354365467,
        0.00944558165086633,
        -0.017212383024394325,
        0.01830710582698195,
        -0.010748182318473543,
        -0.003809417670638022,
        0.020059787670011972,
        -0.0306426420748565,
        0.02894452747150321,
        -0.01214219453278461,
        -0.016683406564116043,
        0.04825691585523149,
        -0.06879210872427258,
        0.06265533931604236,
        -0.012897348533810412,
        -0.11374662843723274,
        0.6011765454616573,
        0.6011765454616573,
        -0.11374662843723274,
        -0.012897348533810412,
        0.06265533931604236,
        -0.06879210872427258,
        0.04825691585523149,
        -0.016683406564116043,
        -0.01214219453278461,
        0.02894452747150321,
        -0.0306426420748565,
        0.020059787670011972,
        -0.003809417670638022,
        -0.010748182318473543,
        0.01830710582698195,
        -0.017212383024394325,
        0.00944558165086633,
        0.0007562479354365467,
        -0.00892158194730952,
        0.012133969863481234,
        -0.009930050039884208,
        0.004087987482699095,
        0.0024426666581136907,
        -0.0069131311342361754,
        0.00785257325051926,
        -0.005459767344046894,
        0.0012548494659758206,
        0.0027530718998398465,
        -0.0049665175529753905,
        0.004787202334285269,
        -0.002697027834370035,
        -0.00012046829336579358,
        0.0023868140851515977,
        -0.0032733721006291735,
        0.002666628896147836,
        -0.0010894932717156326,
        -0.0006298476311485767,
        0.001758397683263153,
        -0.001946857468262416,
        0.0013037060562800817,
        -0.0002619526215376982,
        -0.0006662806500969817,
        0.0011211539754743315,
        -0.001016179206714934,
        0.0005184131163431792,
        0.00007621217304807374,
        -0.0004971931160782956,
        0.0006095914860387138,
        -0.00044126984894593514,
        0.0001334110847936313,
        0.00014754457642863944,
        -0.0002890473752543451,
        0.0002685338232764779,
        -0.00014028199222028285,
        -0.000008622727505666933,
        0.00010668051341248389,
        -0.00012780301900775913,
        0.00008581389247283942,
        -0.00005145857598133435,
        -0.00011693312320218828,
        0.000019221241265146985
    };

    float Update(float Input)
    {
        //store latest sample into the buffer
        Buffer[BufferIndex] = Input;
        //increment buffer and wrap around if necessary
        if(BufferIndex == FilterLength-1)
        {
            //wrap around back to 0
            BufferIndex = 0;
        }
        else
        {
            //increment as normal
            BufferIndex++;
        }
        //reset output to 0 before calculating new output
        Output = 0;

        std::uint8_t SumIndex = BufferIndex;

        for(std::uint8_t n = 0; n<FilterLength; n++)
        {
            //decrement index and wrap if necessary
            if(SumIndex > 0)
            {
                SumIndex--;
            }
            else
            {
                SumIndex = FilterLength - 1;
            }
            //multiply impulse response with shifted input sample, and add to the output
            Output += FIRImpulseResponse[n] * Buffer[SumIndex];
        }
        return Output;
    }
};

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
    void*  SoundBuffer  = malloc(NBytesBuffer); //pointer to the memory addr where SoundBuffer shall be stored
    float* SampleOut    = (float*)SoundBuffer; //we must then cast a type to it
    float  FilterOutput = 0;

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
    // std::string ROM = "../data/roms/smb.nes";

    //UTILITIES
    // std::string ROM = "../data/test_roms/controller.nes";
    // std::string ROM = "../data/test_roms/nestest.nes";
    // std::string ROM = "../data/test_roms/palette_ram.nes";
    // std::string ROM = "../data/test_roms/sprite_ram.nes";
    // std::string ROM = "../data/test_roms/vram_access.nes";

    //AUDIO UTILITIES
    // std::string ROM = "../data/test_roms/noise.nes";
    std::string ROM = "../data/test_roms/square.nes";
    // std::string ROM = "../data/test_roms/triangle.nes";


    bus.ConnectAPU(apu);
    apu.ConnectBus(bus);

    bus.LoadCartridge(ROM);

    std::vector<std::uint8_t> FramebufferPatternTables = ppu.PushPatternTablesToGUI(bus);

    std::uint32_t PPUCycles = 0;
    std::uint32_t CPUCycles = 0;
    std::uint32_t APUCycles = 0;
    float APUFramesSinceLastSample   = 0;
    float NrAPUCyclesPerSample       = (0.5* 1789773/AudioSettings.freq); //nr of APU frames between each sample.
    std::uint16_t AudioSampleCounter = 0;
    std::uint32_t NCyclesPPUWarmUp   = 29658;

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
    const float Frametime = 1000/60;
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
                // *SampleOut++ = FilterOutput; //left channel
                // *SampleOut++ = FilterOutput; //right channel
                AudioSampleCounter++;
                if(AudioSampleCounter == NSamplesBuffer) //checking if the buffer is full
                {
                    // printf("AudioSampleCounter: %03d\n", AudioSampleCounter);
                    // while(SDL_GetQueuedAudioSize(1)){printf("SDL_GetQueuedAudioSize(1): %d\n",SDL_GetQueuedAudioSize(1));}
                    SDL_QueueAudio(1, SoundBuffer, NBytesBuffer); //send full buffer to audio queue
                    AudioSampleCounter = 0; //reset counter
                    // printf("SDL_GetQueuedAudioSize(1): %d\n",SDL_GetQueuedAudioSize(1)/BytesPerSample);
                }
                APUFramesSinceLastSample -= NrAPUCyclesPerSample; //resetting the timer
            }
        }

        NFrames++;
        // printf("New frame\n");
        ppu.FrameComplete = false;

        SDL_RenderClear(Renderer); //clearing the renderer after each frame. required to avoid drawings persisting between frames.

        SDL_SetRenderDrawColor( Renderer, 0x0, 0x00, 0x00, 0x00 ); //setting a background colour

        t2 = SDL_GetTicks();
        // printf("SDL_GetQueuedAudioSize(1): %d\n",SDL_GetQueuedAudioSize(1)/BytesPerSample);
        // while(SDL_GetQueuedAudioSize(1)){}
        // while(SDL_GetQueuedAudioSize(1)){printf("SDL_GetQueuedAudioSize(1): %d\n",SDL_GetQueuedAudioSize(1));}
        //capping framerate
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

#pragma once

#include "../src/bus.h"
#include "../src/noise.h"
#include "../src/pulse.h"
#include "../src/triangle.h"

struct Bus; //forward declaring the bus
struct Noise;
struct Pulse; //forward declaring a pulse channel
struct Triangle;

struct APU
{
    Bus* bus_ptr;
    Noise noise;
    Pulse pulse1;
    Pulse pulse2;
    Triangle triangle;

    //audio parameters for SDL
    //only some are defined here where used in creating waveforms. The rest shall be defined in main
    std::uint32_t SamplingRate = 48000;       //sampling frequency [Hz]
    std::uint8_t  Channels     = 2;           //nr of channels for our audio device. 1 = mono, 2 = stereo, etc

    //initialising variables to control & track the APU
    std::uint32_t Cycle             = 0;
    std::uint8_t  Sequencer         = 0;

    //DMC CHANNEL https://www.nesdev.org/wiki/APU_DMC
    //$4010
    std::uint8_t  DMC_IRQEnabledFlag   = 0;
    std::uint8_t  DMC_LoopFlag         = 0;
    std::uint8_t  DMC_RateIndex        = 0;
    //$4011
    std::uint8_t  DMC_DirectLoad       = 0;
    //$4012
    std::uint16_t DMC_SampleAddr       = 0;
    //$4013
    std::uint16_t DMC_SampleLength     = 0;
    //additional variables to control & track DMC
    std::uint8_t  DMC_IRQFlag          = 0;
    std::uint16_t DMC_Timer            = 0;
    std::uint8_t  DMC_OutputLevel      = 0;
    std::uint8_t  DMC_SilenceFlag      = 0;
    std::uint8_t  DMC_BitsRemaining    = 0;
    std::uint8_t  DMC_BytesRemaining   = 0;
    std::uint8_t  DMC_ShiftRegister    = 0;
    std::uint8_t  DMC_SampleBuffer     = 0;
    std::uint16_t DMC_SampleAddrCurr   = 0;

    //control $4015 write
    std::uint8_t DMC_Enable                   = 0;
    std::uint8_t Noise_LengthCounterEnable    = 0;

    //status $4015 read
    std::uint8_t DMC_InterruptFlag    = 0;
    std::uint8_t FrameInterrupt       = 0;
    std::uint8_t DMC_Active           = 0;

    //mixer parameters
    std::uint16_t Pulse1_Output   =  0;
    std::uint16_t Pulse2_Output   =  0;
    std::uint16_t Triangle_Output =  0;
    std::uint16_t Noise_Output    =  0;
    std::uint16_t DMC_Output      =  0;
    float MixerLUT_Pulse[31]      = {0};
    float MixerLUT_TND[203]       = {0};

    //frame counter
    std::uint8_t FrameCounter5Step     = 0;
    std::uint8_t DisableFrameInterrupt = 0;
    
    //constructor
    APU();
    
    //destructor
    ~APU();
    
    void ConnectBus(Bus&);

    void PopulateMixerLUTs();

    float Mixer();

    void ClockEnvelope_Noise();

    void ClockEnvelopes();

    void ClockSweepUnits();

    void ClockLinearCounter();

    void ClockFrameCounter();

    void ClockLengthCounter();

    std::uint16_t ClockDMC();
    void OutputTimerClock_DMC();
    void EmptyDMCSampleBuffer();

    void Clock();

    //pulse sequencer LUT
    std::uint16_t DMC_RateIndexLUT[16] = {428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54};
};

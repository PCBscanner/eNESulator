#pragma once

#include "../src/bus.h"
#include "../src/dmc.h"
#include "../src/noise.h"
#include "../src/pulse.h"
#include "../src/triangle.h"

struct Bus; //forward declaring the bus
struct DMC;
struct Noise;
struct Pulse; //forward declaring a pulse channel
struct Triangle;

struct APU
{
    Bus* bus_ptr;
    DMC dmc;
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

    //status $4015 read
    std::uint8_t FrameInterrupt       = 0;

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

    void PrefillSampleBuffer();

    void Clock();
};

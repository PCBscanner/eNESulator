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
    const std::uint32_t SamplingRate = 48000;       //sampling frequency [Hz]
    const std::uint8_t  Channels     = 2;           //nr of channels for our audio device. 1 = mono, 2 = stereo, etc

    //initialising variables to control & track the APU
    std::uint32_t Cycle;
    std::uint8_t  Sequencer;

    //status $4015 read
    std::uint8_t FrameInterrupt;

    //mixer parameters
    std::uint16_t Pulse1_Output;
    std::uint16_t Pulse2_Output;
    std::uint16_t Triangle_Output;
    std::uint16_t Noise_Output;
    std::uint16_t DMC_Output;
    float MixerLUT_Pulse[31] = {0};
    float MixerLUT_TND[203]  = {0};

    //frame counter
    std::uint8_t FrameCounter5Step;
    std::uint8_t DisableFrameInterrupt;
    
    //constructor
    APU();
    
    //destructor
    ~APU();
    
    void Reset();

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

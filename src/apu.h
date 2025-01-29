#pragma once

#include "../src/bus.h"

struct APU
{
    //audio parameters for SDL
    //only some are defined here where used in creating waveforms. The rest shall be defined in main
    std::uint32_t SamplingRate = 48000;       //sampling frequency [Hz]
    std::uint8_t Channels      = 2;           //nr of channels for our audio device. 1 = mono, 2 = stereo, etc

    //initialising various variables
    std::uint32_t Cycle             = 0;
    std::uint8_t  LengthCounter     = 0;
    std::uint8_t  LengthCounterHalt = 0;
    std::uint8_t  FrameCounterStep  = 0;
    
    //pulse 1 channel
    std::uint8_t  Pulse1_Duty              = 0;
    std::uint8_t  Pulse1_LengthCounterHalt = 0;
    std::uint8_t  Pulse1_ConstVolume       = 0;
    std::uint8_t  Pulse1_DividerPeriod     = 0;
    std::uint8_t  Pulse1_SweepEnabled      = 0;
    std::uint8_t  Pulse1_SweepPeriod       = 0;
    std::uint8_t  Pulse1_SweepNegative     = 0;
    std::uint8_t  Pulse1_SweepShiftCount   = 0;
    std::uint16_t Pulse1_Timer             = 0;
    std::uint8_t  Pulse1_LengthCounter     = 0;

    //control
    std::uint8_t DMCEnable                    = 0;
    std::uint8_t LengthCounterEnable_Noise    = 0;
    std::uint8_t LengthCounterEnable_Triangle = 0;
    std::uint8_t LengthCounterEnable_Pulse2   = 0;
    std::uint8_t LengthCounterEnable_Pulse1   = 0;

    //status
    std::uint8_t DMCInterrupt           = 0;
    std::uint8_t FrameInterrupt         = 0;
    std::uint8_t DMCActive              = 0;
    std::uint8_t LengthCounter_Noise    = 0;
    std::uint8_t LengthCounter_Triangle = 0;
    std::uint8_t LengthCounter_Pulse2   = 0;
    std::uint8_t LengthCounter_Pulse1   = 0;

    //frame counter
    std::uint8_t FrameCounter5Step     = 0;
    std::uint8_t DisableFrameInterrupt = 0;
    
    //pulse 2 channel
    
    //triangle channel
    
    //noise channel
    
    //DMC channel
    
    //constructor
    APU();
    
    //destructor
    ~APU();
    
    void UpdateAPURegs_FromCPU(Bus&);

    void UpdateAPURegs_ToCPU(Bus&);
    
    void ClockLengthCounter();
    
    void Clock(Bus&);
    
};

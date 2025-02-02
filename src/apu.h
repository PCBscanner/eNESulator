#pragma once

#include "../src/bus.h"

struct Bus; //forward declaring the bus

struct APU
{
    //audio parameters for SDL
    //only some are defined here where used in creating waveforms. The rest shall be defined in main
    std::uint32_t SamplingRate = 48000;       //sampling frequency [Hz]
    std::uint8_t  Channels     = 2;           //nr of channels for our audio device. 1 = mono, 2 = stereo, etc

    //initialising various variables
    std::uint32_t Cycle             = 0;
    std::uint8_t  LengthCounter     = 0;
    std::uint8_t  LengthCounterHalt = 0;
    std::uint8_t  Sequencer         = 0;
    
    //PULSE 1 CHANNEL
    //registers
    //$4000
    std::uint8_t  Pulse1_Duty              = 0;
    std::uint8_t  Pulse1_LengthCounterHalt = 0;
    std::uint8_t  Pulse1_ConstVolumeFlag   = 0;
    std::uint8_t  Pulse1_Vol_EnvPeriod     = 0;
    //$4001
    std::uint8_t  Pulse1_SweepEnabled      = 0;
    std::uint8_t  Pulse1_SweepPeriod       = 0;
    std::uint8_t  Pulse1_SweepNegative     = 0;
    std::uint8_t  Pulse1_SweepShiftCount   = 0;
    //$4002 & $4003
    std::uint16_t Pulse1_TimerLoad         = 0;
    std::uint8_t  Pulse1_LengthCounterLoad = 0;
    std::uint16_t Pulse1_TimerTemp         = 0;
    //additional variables to control & track pulse 1
    std::uint8_t  Pulse1_Sequencer       = 0;
    std::uint8_t  Pulse1_Envelope        = 0;
    std::uint16_t Pulse1_Timer           = 0;
    std::uint8_t  Pulse1_StartFlag       = 0;
    std::uint8_t  Pulse1_EnvelopeDivider = 0;
    std::uint8_t  Pulse1_DecayLvlCounter = 0;
    std::uint8_t  Pulse1_LengthCounter   = 0;
    std::uint8_t  Pulse1_SweepDivider    = 0;
    std::uint8_t  Pulse1_SweepReloadFlag = 0;

    //PULSE 2 CHANNEL
    //registers
    //$4004
    std::uint8_t  Pulse2_Duty              = 0;
    std::uint8_t  Pulse2_LengthCounterHalt = 0;
    std::uint8_t  Pulse2_ConstVolumeFlag   = 0;
    std::uint8_t  Pulse2_Vol_EnvPeriod     = 0;
    //$4005
    std::uint8_t  Pulse2_SweepEnabled      = 0;
    std::uint8_t  Pulse2_SweepPeriod       = 0;
    std::uint8_t  Pulse2_SweepNegative     = 0;
    std::uint8_t  Pulse2_SweepShiftCount   = 0;
    //$4006 & $4007
    std::uint16_t Pulse2_TimerLoad         = 0;
    std::uint8_t  Pulse2_LengthCounterLoad = 0;
    std::uint16_t Pulse2_TimerTemp         = 0;
    //additional variables to control & track pulse 2
    std::uint8_t  Pulse2_Sequencer       = 0;
    std::uint8_t  Pulse2_Envelope        = 0;
    std::uint16_t Pulse2_Timer           = 0;
    std::uint8_t  Pulse2_StartFlag       = 0;
    std::uint8_t  Pulse2_EnvelopeDivider = 0;
    std::uint8_t  Pulse2_DecayLvlCounter = 0;
    std::uint8_t  Pulse2_LengthCounter   = 0;
    std::uint8_t  Pulse2_SweepDivider    = 0;
    std::uint8_t  Pulse2_SweepReloadFlag = 0;

    //TRIANGLE CHANNEL https://www.nesdev.org/wiki/APU_Triangle
    //registers
    //$4008
    std::uint8_t  Triangle_LengthCounterHalt   = 0;
    std::uint8_t  Triangle_CounterReloadVal    = 0;
    //$400A & $400B
    std::uint16_t Triangle_TimerTemp           = 0;
    std::uint16_t Triangle_TimerLoad           = 0;
    std::uint8_t  Triangle_LengthCounterLoad   = 0;
    //additional variables to control & track triangle
    std::uint8_t  Triangle_LinearCounterReload = 0;
    std::uint8_t  Triangle_Sequencer           = 0;
    std::uint16_t Triangle_Timer               = 0;
    std::uint8_t  Triangle_LengthCounter       = 0;
    std::uint8_t  Triangle_LinearCounter       = 0;

    //NOISE CHANNEL https://www.nesdev.org/wiki/APU_Noise
    //$400C
    std::uint8_t  Noise_LengthCounterHalt = 0;
    std::uint8_t  Noise_ConstVolumeFlag   = 0;
    std::uint8_t  Noise_Vol_EnvPeriod     = 0;
    //$400E
    std::uint8_t  Noise_ModeFlag          = 0;
    std::uint8_t  Noise_Period            = 0;
    //$400F
    std::uint8_t  Noise_LengthCounterLoad = 0;
    //additional variables to control & track noise
    std::uint16_t Noise_ShiftRegister     = 1; //On power-up, the shift register is loaded with the value 1.
    std::uint8_t  Noise_Envelope          = 0;
    std::uint16_t Noise_Timer             = 0;
    std::uint8_t  Noise_StartFlag         = 0;
    std::uint8_t  Noise_EnvelopeDivider   = 0;
    std::uint8_t  Noise_DecayLvlCounter   = 0;
    std::uint8_t  Noise_LengthCounter     = 0;

    //DMC CHANNEL https://www.nesdev.org/wiki/APU_DMC
    //$4010
    std::uint8_t  DMC_IRQEnabledFlag = 0;
    std::uint8_t  DMC_LoopFlag       = 0;
    std::uint8_t  DMC_RateIndex      = 0;
    //$4011
    std::uint8_t  DMC_DirectLoad     = 0;
    //$4012
    std::uint8_t  DMC_SampleAddress  = 0;
    //$4013
    std::uint8_t  DMC_SampleLength   = 0;
    //additional variables to control & track DMC
    std::uint8_t  DMC_IRQFlag        = 0;
    std::uint8_t  DMC_SampleBuffer   = 0;
    std::uint16_t DMC_Timer          = 0;
    std::uint8_t  DMC_OutputLevel    = 0;
    std::uint8_t  DMC_Counter        = 0;

    //control $4015 write
    std::uint8_t DMC_Enable                   = 0;
    std::uint8_t Noise_LengthCounterEnable    = 0;
    std::uint8_t Triangle_LengthCounterEnable = 0;
    std::uint8_t Pulse2_LengthCounterEnable   = 0;
    std::uint8_t Pulse1_LengthCounterEnable   = 0;

    //status $4015 read
    std::uint8_t DMCInterrupt           = 0;
    std::uint8_t FrameInterrupt         = 0;
    std::uint8_t DMCActive              = 0;

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
    
    //pulse 2 channel
    
    //triangle channel
    
    //noise channel
    
    //DMC channel
    
    //constructor
    APU();
    
    //destructor
    ~APU();
    
    void PopulateMixerLUTs();

    void UpdateAPURegs_FromCPU(Bus&);

    void UpdateAPURegs_ToCPU(Bus&);

    float Mixer();

    std::uint16_t ClockPulse1();
    std::uint16_t ClockPulse2();
    std::uint16_t ClockTriangle();
    std::uint16_t ClockNoise();

    void ClockEnvelope_Pulse1();
    void ClockEnvelope_Pulse2();
    void ClockEnvelope_Noise();

    void ClockEnvelopes();

    std::uint16_t Sweep_Pulse1();
    std::uint16_t Sweep_Pulse2();

    void ClockSweepUnits();

    void ClockLinearCounter();

    void ClockFrameCounter();

    void ClockLengthCounter();

    void Clock(Bus&);

    //pulse sequencer LUT
    std::uint8_t  PulseSeqLUT[4][8]  = { {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,1,1}, {0,0,0,0,1,1,1,1}, {1,1,1,1,1,1,0,0} };
    std::uint8_t  TriangleSeqLUT[32] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    std::uint16_t NoisePeriodLUT[16] = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};

    std::uint8_t LengthCounterLUT[32] = {10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
                                                12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30};
    
};

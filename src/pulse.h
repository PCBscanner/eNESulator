#pragma once
#include <cstdint>

struct Pulse
{
    //pulse 1 channel registers
    //$4000
    std::uint8_t  Duty              = 0;
    std::uint8_t  LengthCounterHalt = 0;
    std::uint8_t  ConstVolumeFlag       = 0;
    std::uint8_t  Vol_EnvPeriod     = 0;
    //$4001
    std::uint8_t  SweepEnabled      = 0;
    std::uint8_t  SweepPeriod       = 0;
    std::uint8_t  SweepNegative     = 0;
    std::uint8_t  SweepShiftCount   = 0;
    //$4002 & $4003
    std::uint16_t TimerLoad             = 0;
    std::uint8_t  LengthCounterLoad     = 0;
    std::uint16_t TimerTemp         = 0;

    std::uint8_t  Sequencer       = 0;
    std::uint8_t  Envelope       = 0;
    std::uint8_t  Timer      = 0;
    std::uint8_t  StartFlag         = 0;
    std::uint8_t  EnvelopeDivider   = 0;
    std::uint8_t  DecayLvlCounter   = 0;
    std::uint8_t  LengthCounter   = 0;

    std::uint8_t LengthCounterEnable   = 0;
    
    //constructor
    Pulse();
    
    //destructor
    ~Pulse();
    
    std::uint16_t ClockPulse();

    void ClockEnvelopes();

    void ClockLengthCounter();

    //pulse sequencer LUT
    std::uint8_t PulseSeqLUT[4][8] = { {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,1,1}, {0,0,0,0,1,1,1,1}, {1,1,1,1,1,1,0,0} };
    
};

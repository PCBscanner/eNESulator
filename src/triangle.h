#pragma once

#include <cstdint>

struct Triangle //https://www.nesdev.org/wiki/APU_Triangle
{
    //$4008
    std::uint8_t  ControlFlag              = 0;
    std::uint8_t  LinearCounterReloadValue = 0;

    //$400A & $400B
    std::uint16_t TimerPeriod             = 0;
    std::uint8_t  LengthCounterLoad       = 0;
    std::uint8_t  LinearCounterReloadFlag = 0;

    //$4015
    std::uint8_t Enabled = 0;

    //other parameters to track the channel
    std::uint16_t Timer = 0;
    std::uint8_t  Sequencer = 0;
    std::uint8_t  LengthCounter = 0;
    std::uint8_t  LinearCounter = 0;

    std::uint8_t Clock();
    void ClockTimer();
    void ClockWaveformGenerator();
    void ClockLinearCounter();
    void ReloadLengthCounter();
    void ClockLengthCounter();

    std::uint8_t  LengthCounterLUT[32] = {10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
        12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30};
    std::uint8_t  SequencerLUT[32]   = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

};

#pragma once

#include <cstdint>

struct Noise
{
    //$400C
    std::uint8_t LengthCounterHalt         = 0; //this is also the envelope loop flag
    std::uint8_t ConstVolFlag              = 0;
    std::uint8_t Vol_EnvelopeDividerPeriod = 0;

    //$400E
    std::uint8_t ModeFlag    = 0;
    std::uint8_t TimerPeriod = 0;

    //$400F
    std::uint8_t LengthCounterLoad = 0;
    std::uint8_t StartFlag         = 0;

    //$4015
    std::uint8_t Enabled = 0;

    //other parameters to track the channel
    std::uint16_t Timer           = 0;
    std::uint8_t  Sequencer       = 0;
    std::uint8_t  LengthCounter   = 0;
    std::uint16_t ShiftRegister   = 1; //shift register is loaded with 1 on power-up
    std::uint8_t  DecayLvlCounter = 0;
    std::uint8_t  EnvelopeDivider = 0;
    std::uint8_t  EnvelopeOutput  = 0;

    std::uint8_t Clock();
    void ClockTimer();
    void ClockShiftRegister();
    std::uint8_t CalculateFeedback();
    void ReloadLengthCounter();
    void ClockLengthCounter();
    void ClockEnvelope();
    void ClockEnvelopeDivider();
    void ClockDecayLvlCounter();

    std::uint8_t  LengthCounterLUT[32] = {10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
        12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30};
    std::uint16_t TimerPeriodLUT[16]   = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};

};

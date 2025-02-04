#pragma once

#include <cstdint>

struct Pulse
{
    //$4000 or $4004
    std::uint8_t Duty                      = 0;
    std::uint8_t LengthCounterHalt         = 0; //this is also the envelope loop flag
    std::uint8_t ConstVolFlag              = 0;
    std::uint8_t Vol_EnvelopeDividerPeriod = 0;
    //writes to this update the duty cycle. sequencer's current position is not changed.

    //$4001 or $4005
    std::uint8_t  SweepEnabled       = 0;
    std::uint8_t  SweepDividerPeriod = 0;
    std::uint8_t  SweepNegateFlag    = 0;
    std::uint8_t  SweepShiftCount    = 0;
    std::uint8_t  SweepDivider       = 0;
    std::uint8_t  SweepReloadFlag    = 0;
    //Whenever the current period or sweep setting changes, whether by $400x writes or by sweep updating the period, the target period also changes.

    //$4002/3 or $4006/7
    std::uint16_t TimerPeriod       = 0;
    std::uint8_t  LengthCounterLoad = 0;
    std::uint8_t  StartFlag         = 0;
    //sequencer is immediately restarted at the first value of the current sequence. envlope is also restarted. divider (timer?) is NOT reset.

    //$4015
    std::uint8_t Enabled = 0;

    //other parameters to track the channel
    std::uint8_t  Sequencer       = 0;
    std::uint16_t Timer           = 0;
    std::uint8_t  LengthCounter   = 0;
    std::uint8_t  SweepAdjustment = 0;
    std::uint8_t  DecayLvlCounter = 0;
    std::uint8_t  EnvelopeDivider = 0;
    std::uint8_t  EnvelopeOutput  = 0;
    bool Mute = true;

    std::uint8_t Clock();
    void ClockTimer();
    void ClockWaveformGenerator();
    std::uint16_t CalculateTargetPeriod();
    void ClockSweepUnit();
    void ReloadLengthCounter();
    void ClockLengthCounter();
    void ClockEnvelope();
    void ClockEnvelopeDivider();
    void ClockDecayLvlCounter();

    std::uint8_t  LengthCounterLUT[32] = {10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
        12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30};
    std::uint8_t  SequencerLUT[4][8]    = { {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,1,1}, {0,0,0,0,1,1,1,1}, {1,1,1,1,1,1,0,0} };

};


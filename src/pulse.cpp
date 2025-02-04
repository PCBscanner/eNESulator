#include "../src/pulse.h"

std::uint8_t Pulse::Clock()
{
    Mute = ( (TimerPeriod < 8) || (CalculateTargetPeriod() > 0x7FF) || (!LengthCounter) );

    ClockTimer();

    std::uint8_t OutputLevel = EnvelopeOutput * SequencerLUT[Duty][Sequencer];

    return (Mute ? 0 : OutputLevel);
}

void Pulse::ClockTimer()
{
    if(!Timer) //if the timer is zero
    {
        Timer = TimerPeriod;      //reloading the timer with the current period
        ClockWaveformGenerator(); //clocking the waveform generator
    }
    else
    {
        Timer--; //decrementing the timer as normal
    }
}

void Pulse::ClockWaveformGenerator()
{
    Sequencer = ( (Sequencer) ? (Sequencer - 1) : (7) );
}

std::uint16_t Pulse::CalculateTargetPeriod()
{
    std::uint16_t TargetPeriod = 0;
    std::uint16_t ChangeAmount = TimerPeriod >> SweepShiftCount; //shifting the 11-bit raw timer period right by the shift count

    if(SweepNegateFlag)
    {
        TargetPeriod = ( (ChangeAmount > TimerPeriod) ? 0 : (TimerPeriod - ChangeAmount - SweepAdjustment) );
    }
    else
    {
        TargetPeriod = TimerPeriod + ChangeAmount;
    }

    return TargetPeriod;
}

void Pulse::ClockSweepUnit() //https://www.nesdev.org/wiki/APU_Sweep
{
    if(!SweepDivider && SweepEnabled && SweepShiftCount)
    {
        if(!Mute)
        {
            TimerPeriod = CalculateTargetPeriod();
        }
    }

    if(!SweepDivider || SweepReloadFlag)
    {
        SweepDivider = SweepDividerPeriod; //divider counter set to P
        SweepReloadFlag = 0; //clearing the reload flag
    }
    else
    {
        SweepDivider--; //the divider counter is decremented
    }
}

void Pulse::ReloadLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(Enabled)
    {
        LengthCounter = LengthCounterLUT[LengthCounterLoad];
    }
}

void Pulse::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(LengthCounter && !LengthCounterHalt)
    {
        LengthCounter--;
    }
}

void Pulse::ClockEnvelope() //https://www.nesdev.org/wiki/APU_Envelope
{
    if(!StartFlag) //if the start flag is clear
    {
        ClockEnvelopeDivider();
    }
    else
    {
        StartFlag = 0;                               //clearing the start flag
        DecayLvlCounter = 15;                        //reloading decay level counter
        EnvelopeDivider = Vol_EnvelopeDividerPeriod; //divider's period is immediately reloaded
    }

    EnvelopeOutput = (ConstVolFlag ? Vol_EnvelopeDividerPeriod : DecayLvlCounter);
}

void Pulse::ClockEnvelopeDivider()
{
    if(!EnvelopeDivider)
    {
        EnvelopeDivider = Vol_EnvelopeDividerPeriod;
        ClockDecayLvlCounter();
    }
    else
    {
        EnvelopeDivider--;
    }
}

void Pulse::ClockDecayLvlCounter()
{
    if(DecayLvlCounter)    //if decay level counter is non-zero...
    {
        DecayLvlCounter--; //...decrement
    }
    else if(LengthCounterHalt) //otherwise if the loop flash is set...
    {
        DecayLvlCounter = 15; //...the decay level counter is loaded with 15
    }
}

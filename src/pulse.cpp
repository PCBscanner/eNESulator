#include "../src/pulse.h"

Pulse::Pulse()
{
    //constructor in here
}

Pulse::~Pulse()
{
    //destructor in here
}

std::uint16_t Pulse::ClockPulse()
{
    // printf("LengthCounterLoad %d, TimerLoad %d, Timer %d, Sequencer%d\n", LengthCounter, Timer, Timer, Sequencer);
    //sweep not yet implemented
    if( (LengthCounterLoad == 0) || (TimerLoad == 0) )
    {
        return 0;
    }
    if(Timer == 0)
    {
        Timer = TimerLoad; //reset to the timer value
        if(Sequencer == 0)
        {
            Sequencer = 7; //resetting to 7
        }
        else
        {
            Sequencer--; //decrement normally
        }
    }
    else
    {
        Timer--; //decrement normally
    }

    return (ConstVolumeFlag ? Vol_EnvPeriod : DecayLvlCounter) * PulseSeqLUT[Duty][Sequencer];

}

void Pulse::ClockEnvelopes()
{
    if(!StartFlag) //start flag clear. clock divider.
    {
        if(!EnvelopeDivider) //clocking divider whilst at zero.
        {
            EnvelopeDivider = Vol_EnvPeriod; //reload with V
            if(DecayLvlCounter) //non-zero counter
            {
                DecayLvlCounter--; //only decrement
            }
            else
            {
                if(LengthCounterHalt) //Halt length counter (this bit is also the envelope's loop flag)
                {
                    DecayLvlCounter = 15;
                }
            }
        }
        else
        {
            EnvelopeDivider--;
        }
    }
    else
    {
        StartFlag = 0; //clear start flag
        DecayLvlCounter = 15;
        EnvelopeDivider = Vol_EnvPeriod; //divider period reloaded
    }
}

void Pulse::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(!LengthCounterEnable) //clearing enabled bit
    {
        LengthCounter = 0;
    }
    if( LengthCounter && !LengthCounterHalt )
    {
        LengthCounter--;
    }
}

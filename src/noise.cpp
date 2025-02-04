#include "../src/noise.h"

std::uint8_t Noise::Clock()
{
    ClockTimer();

    std::uint8_t OutputLevel = 0;

    if( !(ShiftRegister & 1) || !LengthCounter )
    {
        OutputLevel = 0;
    }
    else
    {
        OutputLevel = EnvelopeOutput;
    }

    return OutputLevel;
}

void Noise::ClockTimer()
{
    if(!Timer) //if the timer is zero
    {
        //sequencer is clocked by the timer as long as both the linear counter and the length counter are non-zero
        if( LengthCounter )
        {
            ClockShiftRegister();    //clocking the sequencer
        }
        Timer = TimerPeriod; //reloading the timer (+ 1)
    }
    else
    {
        Timer--; //decrementing the timer as normal
    }
}

void Noise::ClockShiftRegister()
{
    std::uint8_t Feedback = CalculateFeedback();
    ShiftRegister >>= 1; //shifting register right by one bit
    ShiftRegister |= (Feedback << 14);
}

std::uint8_t Noise::CalculateFeedback()
{
    std::uint8_t Feedback = 0;
    std::uint8_t EORBit = ( ModeFlag ? ( (ShiftRegister & 0b1000000) >> 6 ) : ( (ShiftRegister & 0b10) >> 1 ) );
    Feedback = (ShiftRegister & 0b1) ^ EORBit;
    return Feedback;
}

void Noise::ReloadLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(Enabled)
    {
        LengthCounter = LengthCounterLUT[LengthCounterLoad];
    }
}

void Noise::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(LengthCounter && !LengthCounterHalt)
    {
        LengthCounter--;
    }
}

void Noise::ClockEnvelope() //https://www.nesdev.org/wiki/APU_Envelope
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

void Noise::ClockEnvelopeDivider()
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

void Noise::ClockDecayLvlCounter()
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

#include "../src/triangle.h"
#include <fstream>

std::uint8_t Triangle::Clock()
{
    ClockTimer();

    std::uint8_t OutputLevel = SequencerLUT[Sequencer];

    return OutputLevel;
}

void Triangle::ClockTimer()
{
    if(!Timer) //if the timer is zero
    {
        //sequencer is clocked by the timer as long as both the linear counter and the length counter are non-zero
        if( (LinearCounter) && (LengthCounter) )
        {
            ClockWaveformGenerator();    //clocking the sequencer
        }
        Timer = TimerPeriod + 1; //reloading the timer (+ 1)
    }
    else
    {
        Timer--; //decrementing the timer as normal
    }
}

void Triangle::ClockWaveformGenerator()
{
    Sequencer = ( (Sequencer) ? (Sequencer - 1) : (31) );
}

void Triangle::ClockLinearCounter()
{
    if(LinearCounterReloadFlag)
    {
        LinearCounter = LinearCounterReloadValue;
    }
    else if(LinearCounter)
    {
        LinearCounter--;
    }
    if(!ControlFlag) //if the control flag is clear...
    {
        LinearCounterReloadFlag = 0; //... we clear the linear counter reload flag
    }
}

void Triangle::ReloadLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(Enabled)
    {
        LengthCounter = LengthCounterLUT[LengthCounterLoad];
    }
}

void Triangle::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(LengthCounter && !ControlFlag) //the "linear counter control" flag is also the "halt length counter" flag
    {
        LengthCounter--;
    }
}

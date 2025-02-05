#include "../src/dmc.h"

std::uint8_t DMC::Clock()
{
    ClockTimer();
    
    return (SilenceFlag ? 0 : OutputLevel);
}

void DMC::ClockTimer()
{
    if(!Timer)
    {
        if(!SilenceFlag) //if the silence flag is clear...
        {
            UpdateOutputLevel(); //output level changes based on bit 0
        }
        ClockShiftRegister();
        UpdateBitsRemaining();
        Timer = RateLUT[RateIndex] >> 1; //reload timer with the rate index.
    }
    else
    {
        Timer--;
    }
}

void DMC::UpdateOutputLevel()
{
    if( (ShiftRegister & 1) && (OutputLevel <= 125) )
    {
        OutputLevel += 2;
    }
    else if( !(ShiftRegister & 1) && (OutputLevel >= 2) )
    {
        OutputLevel -= 2;
    }
}

void DMC::ClockShiftRegister()
{
    ShiftRegister >>= 1;
}

void DMC::UpdateBitsRemaining()
{
    if(BitsRemaining)
    {
        BitsRemaining--;
    }
    if(!BitsRemaining)
    {
        StartNewOutputCycle();
    }
}

void DMC::StartNewOutputCycle()
{
    BitsRemaining = 8; //load the counter with 8
    if(!SampleBuffer)
    {
        SilenceFlag = 1;
    }
    else
    {
        SilenceFlag = 0;
        EmptySampleBuffer();
    }
}

void DMC::EmptySampleBuffer()
{
    ShiftRegister = SampleBuffer;
    if(BytesRemaining)
    {
        RefillSampleBuffer();
    }
}

void DMC::RefillSampleBuffer()
{
    // todo The CPU is stalled for 1-4 CPU cycles to read a sample byte. The exact cycle count depends on many factors and is described in detail in the DMA article.
    
    // SampleBuffer = bus_ptr->CPUMemory[CurrentAddr]; //todo need mapping here.

    SampleBuffer = SampleBufferNext; //see apu.cpp for explanation...
    
    CurrentAddr = ( (CurrentAddr == 0xFFFF) ? 0x8000 : (CurrentAddr + 1) );
    
    DecrementBytesRemaining();
}

void DMC::DecrementBytesRemaining()
{
    BytesRemaining--;
    if(!BytesRemaining && LoopFlag)
    {
        //restart the sample
        CurrentAddr    = SampleAddr;
        BytesRemaining = SampleLength;
    }
    else if(!BytesRemaining && IRQEnabledFlag)
    {
        //set the IRQ flag
        IRQFlag = 1;
    }
}

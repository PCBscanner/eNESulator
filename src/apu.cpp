#include "../src/apu.h"

APU::APU()
{
    PopulateMixerLUTs();
    pulse1.SweepAdjustment = 1;
}

APU::~APU()
{
    //destructor in here
}

void APU::ConnectBus(Bus& bus)
{
    bus_ptr = &bus;
}

void APU::PopulateMixerLUTs() //https://www.nesdev.org/wiki/APU_Mixer
{
    for(std::uint8_t n = 0; n<31; n++)
    {
        MixerLUT_Pulse[n] = (95.52 / ( (8128.0/static_cast<float>(n)) + 100 ));
    }

    for(std::uint8_t n = 0; n<203; n++)
    {
        MixerLUT_TND[n] = (163.67 / ( (24329.0/static_cast<float>(n)) + 100 ));
    }
}

float APU::Mixer() //https://www.nesdev.org/wiki/APU_Mixer
{
    return ( MixerLUT_Pulse[Pulse1_Output + Pulse2_Output] + MixerLUT_TND[ (3*Triangle_Output) + (2*Noise_Output) + DMC_Output ] );
}

void APU::ClockEnvelopes()
{
    pulse1.ClockEnvelope();
    pulse2.ClockEnvelope();
    noise.ClockEnvelope();
}

void APU::ClockLinearCounter()
{
    triangle.ClockLinearCounter();
}

void APU::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    pulse1.ClockLengthCounter();
    pulse2.ClockLengthCounter();
    triangle.ClockLengthCounter();
    noise.ClockLengthCounter();
}

void APU::ClockSweepUnits()
{
    pulse1.ClockSweepUnit();
    pulse2.ClockSweepUnit();
}

void APU::ClockFrameCounter() //https://www.nesdev.org/wiki/APU_Frame_Counter
{
    if(!FrameCounter5Step) //4-step sequence
    {
        if( (Cycle >= 3729) && Sequencer == 0 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 7457) && Sequencer == 1 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            ClockSweepUnits();
            ClockLengthCounter();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 11186) && Sequencer == 2 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 14914) && Sequencer == 3 )
        {
            //set frame interrupt flag if interrupt inhibit is clear
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 14915) && Sequencer == 4 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            ClockSweepUnits();
            ClockLengthCounter();
            //set frame interrupt flag if interrupt inhibit is clear
            Sequencer = 0;
            Cycle = 0;
        }
        else
        {
            Cycle++;
        }
    }
    else //5-step sequence
    {
        if( (Cycle >= 3729) && Sequencer == 0 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 7457) && Sequencer == 1 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            ClockSweepUnits();
            ClockLengthCounter();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 11186) && Sequencer == 2 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 14915) && Sequencer == 3 )
        {
            Sequencer++;
            Cycle++;
        }
        else if( (Cycle >= 18641) && Sequencer == 4 )
        {
            ClockLinearCounter();
            ClockEnvelopes();
            ClockSweepUnits();
            ClockLengthCounter();
            Sequencer = 0;
            Cycle = 0;
        }
        else
        {
            Cycle++;
        }
    }
}

std::uint16_t APU::ClockDMC()
{
    DMC_Timer--;

    if(!DMC_Timer) //if the timer is zero, we output a clock
    {
        DMC_Timer = DMC_RateIndexLUT[DMC_RateIndex]; //re-load the timer
        OutputTimerClock_DMC();
    }
    return 0;
    return DMC_OutputLevel;
}

void APU::OutputTimerClock_DMC()
{
    if(!DMC_SilenceFlag) //if silence flag is clear
    {
        if( (DMC_ShiftRegister & 1) && (DMC_OutputLevel <= 125) )
        {
            DMC_OutputLevel+=2;
        }
        else if( !(DMC_ShiftRegister & 1) && (DMC_OutputLevel >= 2) )
        {
            DMC_OutputLevel-=2;
        }
    }

    DMC_ShiftRegister >>= 1;    //clocking right shift register by one

    DMC_BitsRemaining--;        //decrementing the bits remaining counter

    if(!DMC_BitsRemaining)      //output cycle has ended
    {
        DMC_BitsRemaining = 8;  //re-load with 8
        if(!DMC_SampleBuffer)
        {
            DMC_SilenceFlag = 1; //set silence flag
        }
        else
        {
            DMC_SilenceFlag = 0; //clear silence flag
            EmptyDMCSampleBuffer();
        }
    }
}

void APU::EmptyDMCSampleBuffer()
{
    DMC_ShiftRegister = DMC_SampleBuffer; //sample buffer emptied into shift register
    DMC_SampleBuffer  = 0;

    if( !DMC_SampleBuffer && DMC_BytesRemaining )
    {
        //sample buffer is filled with the next sample byte read from the current address, subject to whatever mapping hardware is present.
        DMC_SampleBuffer = bus_ptr->CPUMemory[DMC_SampleAddrCurr];
        // DMC_SampleBuffer = bus_ptr->ReadCPUBus(DMC_SampleAddrCurr); //need to sort out pushing a PPU class to this. todo future...

        //address is incremented; if it exceeds $FFFF, it is wrapped around to $8000.
        DMC_SampleAddrCurr = ( (DMC_SampleAddrCurr == 0xFFFF) ? 0x8000 : ( DMC_SampleAddrCurr + 1 ) );

        DMC_BytesRemaining--; //decrementing the bytes remaining counter

        if( !DMC_BytesRemaining && DMC_LoopFlag )
        {
            DMC_SampleAddrCurr   = DMC_SampleAddr;   //current address set to the sample address
            DMC_BytesRemaining   = DMC_SampleLength; //bytes remaining set to the sample length
        }
        else if( !DMC_BytesRemaining && DMC_IRQEnabledFlag )
        {
            DMC_InterruptFlag = 1; //setting the interrupt flag
        }
    }
}

void APU::Clock()
{
    Pulse1_Output   = pulse1.Clock();
    Pulse2_Output   = pulse2.Clock();
    Triangle_Output = triangle.Clock();
    Triangle_Output = triangle.Clock(); //must clock twice per APU cycle. todo find a better way to implement this...
    Noise_Output    = noise.Clock();
    // DMC_Output      = ClockDMC();

    ClockFrameCounter();
}

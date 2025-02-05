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

//using this function to avoid creating a pointer to the bus in the DMC function
//it should work, but we shall see...
void APU::PrefillSampleBuffer()
{
    std::uint16_t NextDMCSampleAddr = ( (dmc.CurrentAddr == 0xFFFF) ? 0x8000 : dmc.CurrentAddr + 1 );
    dmc.SampleBufferNext = bus_ptr->CPUMemory[NextDMCSampleAddr];
}

void APU::Clock()
{
    PrefillSampleBuffer();
    Pulse1_Output   = pulse1.Clock();
    Pulse2_Output   = pulse2.Clock();
    Triangle_Output = triangle.Clock();
    Triangle_Output = triangle.Clock(); //must clock twice per APU cycle. todo find a better way to implement this...
    Noise_Output    = noise.Clock();
    DMC_Output      = dmc.Clock();
    DMC_Output      = dmc.Clock(); //clocking twice to account for the rate being per CPU cycle, not APU cycle

    ClockFrameCounter();
}

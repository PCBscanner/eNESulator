#include "../src/apu.h"

APU::APU()
{
    Pulse pulse1; //initializing a Pulse struct for pulse1
    PopulateMixerLUTs();
}

APU::~APU()
{
    //destructor in here
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

void APU::UpdateAPURegs_ToCPU(Bus& bus)
{
    //control
    bus.CPUMemory[0x4015] |= (DMCInterrupt           << 7);
    bus.CPUMemory[0x4015] |= (FrameInterrupt         << 6);
    bus.CPUMemory[0x4015] |= (DMCActive              << 4);
    bus.CPUMemory[0x4015] |= (LengthCounter_Noise    << 3);
    bus.CPUMemory[0x4015] |= (LengthCounter_Triangle << 2);
    bus.CPUMemory[0x4015] |= (LengthCounter_Pulse2   << 1);
    bus.CPUMemory[0x4015] |= (LengthCounter_Pulse1);
}

float APU::Mixer() //https://www.nesdev.org/wiki/APU_Mixer
{
    return ( MixerLUT_Pulse[Pulse1_Output + Pulse2_Output] + MixerLUT_TND[ (3*Triangle_Output) + (2*Noise_Output) + DMC_Output ] );
}

std::uint16_t APU::ClockPulse1()
{
    if( (Pulse1_LengthCounter == 0) || (Pulse1_TimerLoad < 8) )
    {
        return 0;
    }
    if(Pulse1_Timer == 0)
    {
        Pulse1_Timer = Pulse1_TimerLoad; //reset to the timer value
        if(Pulse1_Sequencer == 0)
        {
            Pulse1_Sequencer = 7; //resetting to 7
        }
        else
        {
            Pulse1_Sequencer--; //decrement normally
        }
    }
    else
    {
        Pulse1_Timer--; //decrement normally
    }

    return (Pulse1_ConstVolumeFlag ? Pulse1_Vol_EnvPeriod : Pulse1_DecayLvlCounter) * PulseSeqLUT[Pulse1_Duty][Pulse1_Sequencer];
}

std::uint16_t APU::ClockPulse2()
{
    if( (Pulse2_LengthCounter == 0) || (Pulse2_TimerLoad < 8) )
    {
        return 0;
    }
    if(Pulse2_Timer == 0)
    {
        Pulse2_Timer = Pulse2_TimerLoad; //reset to the timer value
        if(Pulse2_Sequencer == 0)
        {
            Pulse2_Sequencer = 7; //resetting to 7
        }
        else
        {
            Pulse2_Sequencer--; //decrement normally
        }
    }
    else
    {
        Pulse2_Timer--; //decrement normally
    }

    return (Pulse2_ConstVolumeFlag ? Pulse2_Vol_EnvPeriod : Pulse2_DecayLvlCounter) * PulseSeqLUT[Pulse2_Duty][Pulse2_Sequencer];
}

std::uint16_t APU::ClockTriangle()
{
    if( (Triangle_LengthCounter == 0) || (Triangle_LinearCounter == 0) ) //do not clock sequencer
    {
        return 0;
    }
    //we only clock the sequencer if the timer is 0
    if(Triangle_Timer == 0)
    {
        Triangle_Timer = Triangle_TimerLoad; //reset to the timer value
        if(Triangle_Sequencer == 0)
        {
            Triangle_Sequencer = 31; //resetting to 31
        }
        else
        {
            Triangle_Sequencer--; //decrement normally
        }
    }
    else
    {
        Triangle_Timer--; //decrement normally
    }

    return TriangleSeqLUT[Triangle_Sequencer];
}

void APU::ClockEnvelope_Pulse1()
{
    if(!Pulse1_StartFlag) //start flag clear. clock divider.
    {
        if(!Pulse1_EnvelopeDivider) //clocking divider whilst at zero.
        {
            Pulse1_EnvelopeDivider = Pulse1_Vol_EnvPeriod; //reload with V
            if(Pulse1_DecayLvlCounter) //non-zero counter
            {
                Pulse1_DecayLvlCounter--; //only decrement
            }
            else
            {
                if(Pulse1_LengthCounterHalt) //Halt length counter (this bit is also the envelope's loop flag)
                {
                    Pulse1_DecayLvlCounter = 15;
                }
            }
        }
        else
        {
            Pulse1_EnvelopeDivider--;
        }
    }
    else
    {
        Pulse1_StartFlag = 0; //clear start flag
        Pulse1_DecayLvlCounter = 15;
        Pulse1_EnvelopeDivider = Pulse1_Vol_EnvPeriod; //divider period reloaded
    }
}

void APU::ClockEnvelope_Pulse2()
{
    if(!Pulse2_StartFlag) //start flag clear. clock divider.
    {
        if(!Pulse2_EnvelopeDivider) //clocking divider whilst at zero.
        {
            Pulse2_EnvelopeDivider = Pulse2_Vol_EnvPeriod; //reload with V
            if(Pulse2_DecayLvlCounter) //non-zero counter
            {
                Pulse2_DecayLvlCounter--; //only decrement
            }
            else
            {
                if(Pulse2_LengthCounterHalt) //Halt length counter (this bit is also the envelope's loop flag)
                {
                    Pulse2_DecayLvlCounter = 15;
                }
            }
        }
        else
        {
            Pulse2_EnvelopeDivider--;
        }
    }
    else
    {
        Pulse2_StartFlag = 0; //clear start flag
        Pulse2_DecayLvlCounter = 15;
        Pulse2_EnvelopeDivider = Pulse2_Vol_EnvPeriod; //divider period reloaded
    }
}

void APU::ClockEnvelopes()
{
    ClockEnvelope_Pulse1();
    ClockEnvelope_Pulse2();
}

std::uint16_t APU::Sweep_Pulse1() //https://www.nesdev.org/wiki/APU_Sweep
{
    std::uint16_t ChangeAmount = Pulse1_Timer >> Pulse1_SweepShiftCount;
    std::uint16_t TargetPeriod = 0;
    if(Pulse1_SweepNegative) //if negative flag set
    {
        if(ChangeAmount > Pulse1_Timer)
        {
            TargetPeriod = 0;
        }
        else
        {
            TargetPeriod = Pulse1_Timer - ChangeAmount;
        }
    }
    else
    {
        TargetPeriod = Pulse1_Timer + ChangeAmount;
    }
    return TargetPeriod;
}

void APU::ClockSweepUnits() //https://www.nesdev.org/wiki/APU_Sweep
{
    std::uint16_t Pulse1_TargetPeriod = Sweep_Pulse1();

    if( (!Pulse1_SweepDivider) && (Pulse1_SweepEnabled)  && (Pulse1_SweepShiftCount) )
    {
        if(1) //todo sweep unit is NOT muting the channel
        {
            Pulse1_TimerLoad = Pulse1_TargetPeriod;
        }
        else //todo sweep unit is muting the channel
        {
            //continue as normal? "the pulse's period remains unchanged, but the sweep unit's divider continues to count down and reload the divider's period as normal."
        }
    }
    if( (!Pulse1_SweepDivider) || (Pulse1_SweepReloadFlag) )
    {
        Pulse1_SweepDivider    = Pulse1_SweepPeriod; //divider counter is set to P...
        Pulse1_SweepReloadFlag = 0;                  //... and the reload flag is cleared
    }
    else
    {
        Pulse1_SweepDivider--;                       //otherwise, the divider counter is decremented.
    }
}

void APU::ClockLinearCounter()
{
    if(Triangle_LinearCounterReload)
    {
        Triangle_LinearCounter = Triangle_CounterReloadVal;
        // printf("Triangle_LinearCounter: 0b%08b\n", Triangle_LinearCounter);
    }
    else
    {
        if(Triangle_LinearCounter) //only decrement the triangle linear counter if it is non-zero
        {
            Triangle_LinearCounter--;
        }
    }
    if(!Triangle_ControlFlag) //only clear the linear counter reload flag if the control flag is also clear
    {
        Triangle_LinearCounterReload = 0;
    }
}

void APU::ClockLengthCounter() //https://www.nesdev.org/wiki/APU_Length_Counter
{
    if(!LengthCounterEnable_Pulse1) //clearing enabled bit
    {
        Pulse1_LengthCounter = 0;
    }
    if( Pulse1_LengthCounter && !Pulse1_LengthCounterHalt )
    {
        Pulse1_LengthCounter--;
    }
    if(!LengthCounterEnable_Pulse2) //clearing enabled bit
    {
        Pulse2_LengthCounter = 0;
    }
    if( Pulse2_LengthCounter && !Pulse2_LengthCounterHalt )
    {
        Pulse2_LengthCounter--;
    }
    if(!LengthCounterEnable_Triangle) //clearing enabled bit
    {
        Triangle_LengthCounter = 0;
    }
    if( Triangle_LengthCounter && !Triangle_ControlFlag )
    {
        Triangle_LengthCounter--;
    }
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

void APU::Clock(Bus& bus)
{
    Pulse1_Output   = ClockPulse1();
    Pulse2_Output   = ClockPulse2();
    Triangle_Output = ClockTriangle();
    Triangle_Output = ClockTriangle(); //must clock twice per APU cycle. todo find a better way to implement this...

    ClockFrameCounter();

    //must call this last to update all registers
    UpdateAPURegs_ToCPU(bus);
}

#include "../src/apu.h"

APU::APU()
{
    //constructor in here
}

APU::~APU()
{
    //destructor in here
}

void APU::UpdateAPURegs_FromCPU(Bus& bus)
{
    //update the various values from the last CPU cycles in here
    //pulse 1 channel
    //https://www.nesdev.org/wiki/APU_Pulse
    Pulse1_Duty              = (bus.CPUMemory[0x4000] & 0b11000000) >> 6;
    Pulse1_LengthCounterHalt = (bus.CPUMemory[0x4000] & 0b00100000) >> 5;
    Pulse1_ConstVolume       = (bus.CPUMemory[0x4000] & 0b00010000) >> 4;
    Pulse1_DividerPeriod     = (bus.CPUMemory[0x4000] & 0b00001111);
    Pulse1_SweepEnabled      = (bus.CPUMemory[0x4001] & 0b10000000) >> 7;
    Pulse1_SweepPeriod       = (bus.CPUMemory[0x4001] & 0b01110000) >> 4;
    Pulse1_SweepNegative     = (bus.CPUMemory[0x4001] & 0b00001000) >> 3;
    Pulse1_SweepShiftCount   = (bus.CPUMemory[0x4001] & 0b00000111);
    Pulse1_Timer           = ( (bus.CPUMemory[0x4003] & 0b00000111) << 8 ) | bus.CPUMemory[0x4002];
    Pulse1_LengthCounter     = (bus.CPUMemory[0x4003] & 0b11111000) >> 3;

    //control
    DMCEnable                    = (bus.CPUMemory[0x4015] & 0b00010000) >> 4;
    LengthCounterEnable_Noise    = (bus.CPUMemory[0x4015] & 0b00001000) >> 3;
    LengthCounterEnable_Triangle = (bus.CPUMemory[0x4015] & 0b00000100) >> 2;
    LengthCounterEnable_Pulse2   = (bus.CPUMemory[0x4015] & 0b00000010) >> 1;
    LengthCounterEnable_Pulse1   = (bus.CPUMemory[0x4015] & 0b00000001);

    //frame counter
    FrameCounter5Step     = (bus.CPUMemory[0x4017] & 0b10000000) >> 7;
    DisableFrameInterrupt = (bus.CPUMemory[0x4017] & 0b01000000) >> 6;
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

void APU::ClockLengthCounter()
{
    //https://www.nesdev.org/wiki/APU_Length_Counter
    if(LengthCounterHalt)
    {
        //do something in here
    }
}

void APU::Clock(Bus& bus)
{
    
    UpdateAPURegs_FromCPU(bus);
    //need to modify "if(Cycle >= #)" to account for skipping over the equals
    //https://www.nesdev.org/wiki/APU_Frame_Counter
    if(!FrameCounter5Step) //4-step sequence
    {
        if( (Cycle >= 3729) && FrameCounterStep == 0 )
        {
            //clock envelope & triangle linear counter
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 7457) && FrameCounterStep == 1 )
        {
            //clock envelope & triangle linear counter
            //clock length counters & sweep units
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 11186) && FrameCounterStep == 2 )
        {
            //clock envelope & triangle linear counter
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 14914) && FrameCounterStep == 3 )
        {
            //set frame interrupt flag if interrupt inhibit is clear
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 14915) && FrameCounterStep == 4 )
        {
            //clock envelope & triangle linear counter
            //clock length counters & sweep units
            //set frame interrupt flag if interrupt inhibit is clear
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep = 0;
            Cycle = 0;
        }
        else
        {
            Cycle++;
        }
    }
    else //5-step sequence
    {
        if( (Cycle >= 3729) && FrameCounterStep == 0 )
        {
            //clock envelope & triangle linear counter
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 7457) && FrameCounterStep == 1 )
        {
            //clock envelope & triangle linear counter
            //clock length counters & sweep units
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 11186) && FrameCounterStep == 2 )
        {
            //clock envelope & triangle linear counter
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 14915) && FrameCounterStep == 3 )
        {
            //clock envelope & triangle linear counter
            //clock length counters & sweep units
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep++;
            Cycle++;
        }
        else if( (Cycle >= 18641) && FrameCounterStep == 4 )
        {
            //clock envelope & triangle linear counter
            //clock length counters & sweep units
            printf("Cycle >= %05d && FrameCounterStep == %d\n", Cycle, FrameCounterStep);
            FrameCounterStep = 0;
            Cycle = 0;
        }
        else
        {
            Cycle++;
        }
    }
}

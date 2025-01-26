void CanJumpToASubroutineAndJumpBack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_JSR;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    //We need to set the address (minus one) of the return point to the stack.
    //We then set the PC to the target memory address (0x8000).
    bus.CPUMemory[0x8000] = CPU::INS_RTS; //returning to 0xFF03 (SP + 1)
    bus.CPUMemory[0xFF03] = CPU::INS_LDA_IM;
    bus.CPUMemory[0xFF04] = 0x42;
    std::uint32_t CyclesExpected = 6 + 6 + 2; //6 for JSR. 6 for RTS, 2 for LDA_IM
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    for(int i = 1; i < 4; i++)
    {
        cpu.Execute(Cycles, bus);
    }
    TEST_CHECK(cpu.A == 0x42);
    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.SP == CPUInit.SP);
}

void JSRDoesNotAffectProcessorStatus()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_JSR;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.PC == 0x8000);
    TEST_CHECK(cpu.SP != CPUInit.SP);
    TEST_CHECK(cpu.PS == CPUInit.PS);
}

void RTSDoesNotAffectProcessorStatus()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_JSR;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = CPU::INS_RTS;
    std::uint32_t CyclesExpected = 6 + 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    for(int i = 1; i < 3; i++)
    {
        cpu.Execute(Cycles, bus);
    }

    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.PS == CPUInit.PS);
}

void JumpAbsCanJumpToANewLocationInProgram()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_JMP_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    //we will then jump to location [0x8000]
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0x8000);
    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.SP == CPUInit.SP);
}

void JumpIndCanJumpToANewLocationInProgram()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PC = 0x011F;
    bus.CPUMemory[0x011F] = CPU::INS_JMP_IND;
    bus.CPUMemory[0x0120] = 0xFC;
    bus.CPUMemory[0x0121] = 0xBA;
    //jump 1 to 0xBAFC
    bus.CPUMemory[0xBAFC] = 0x00;
    bus.CPUMemory[0xBAFD] = 0x80;
    //we will then finally jump to location [0x8000]
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    TEST_CHECK(cpu.PC == 0x8000);
    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.SP == CPUInit.SP);
}

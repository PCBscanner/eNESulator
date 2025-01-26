void NOPCanDoNothingButConsumeOneCycle()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000001; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_NOP;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF01);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(cpu.A == CPUInit.A);
    TEST_CHECK(cpu.X == CPUInit.X);
    TEST_CHECK(cpu.Y == CPUInit.Y);
    TEST_CHECK(cpu.SP == CPUInit.SP);
    TEST_CHECK(Cycles == CyclesExpected);
}

void BRKWillLoadTheProgramCounterFromTheInterruptVector()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x00;
    bus.CPUMemory[0xFFFF] = 0x80;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0x8000);
}

void BRKWillStoreTheProgramCounterInTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x00;
    bus.CPUMemory[0xFFFF] = 0x80;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x01FE] == 0x02);
    TEST_CHECK(bus.CPUMemory[0x01FF] == 0xFF);
}

void BRKWillStoreTheProcessorStatusInTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x11;
    cpu.X = 0x22;
    cpu.Y = 0x33;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x22;
    bus.CPUMemory[0xFFFF] = 0x33;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x01FD] == 0b00110000);
}

void BRKWillCorrectlyPushThreeBytesOntoTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x11;
    cpu.X = 0x22;
    cpu.Y = 0x33;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x22;
    bus.CPUMemory[0xFFFF] = 0x33;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.SP == 0xFC);
}

void BRKWillSetTheInterruptDisableFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x11;
    cpu.X = 0x22;
    cpu.Y = 0x33;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x22;
    bus.CPUMemory[0xFFFF] = 0x33;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b00000100) != 0);
}

void BRKWillSetTheBFlagOnTheProcessorStatusInTheStackOnly()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x11;
    cpu.X = 0x22;
    cpu.Y = 0x33;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x22;
    bus.CPUMemory[0xFFFF] = 0x33;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b00010000) == 0); //testing it has not been set on the actual processor status
    TEST_CHECK((bus.CPUMemory[0x01FD] & 0b00010000) != 0); //testing it has been set on the processor status stored in the stack
}

void BRKWillTakeTheCorrectNumberOfCycles()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x11;
    cpu.X = 0x22;
    cpu.Y = 0x33;
    cpu.PC = 0xFF00;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_BRK;
    bus.CPUMemory[0xFFFE] = 0x22;
    bus.CPUMemory[0xFFFF] = 0x33;

    std::uint32_t CyclesExpected = 7 + 7;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(Cycles == CyclesExpected);
}

void RTIWillLoadTheStackPointerAndProgramCounterFromTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b11000000;
    cpu.PC = 0x8000;
    cpu.SP = 0xFC;
    bus.CPUMemory[0x8000] = CPU::INS_RTI;
    bus.CPUMemory[0x01FD] = 0b00110000;
    bus.CPUMemory[0x01FE] = 0x01;
    bus.CPUMemory[0x01FF] = 0xFF;

    std::uint32_t CyclesExpected = 7 + 6;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.PC == 0xFF01);
    TEST_CHECK(cpu.PS == 0b00100000);
}

void NMIWillCorrectlyPushThreeBytesOntoTheStack()
{
    Mem cpuMemory;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b11000000;
    cpu.PC = 0x8056;
    cpu.SP = 0xFF;
    bus.CPUMemory[0xFFFA] = 0x22;
    bus.CPUMemory[0xFFFB] = 0x33;
    std::uint32_t CyclesExpected = 7 + 7;

    cpu.ExecuteNMI(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x01FF] == 0x80);
    TEST_CHECK(bus.CPUMemory[0x01FE] == 0x56);
    TEST_CHECK(bus.CPUMemory[0x01FD] == 0b11000000);

    TEST_CHECK(Cycles == CyclesExpected);
    TEST_CHECK(cpu.PC == 0x3322);
}

void CLCCanClearCarryFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000001; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CLC;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1111110) == (CPUInit.PS & 0b11111110)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000001) == 0); //checking flag has been cleared
    TEST_CHECK(Cycles == CyclesExpected);
}

void CLDCanClearDecimalModeFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00001000; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CLD;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1110111) == (CPUInit.PS & 0b1110111)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00001000) == 0); //checking flag has been cleared
    TEST_CHECK(Cycles == CyclesExpected);
}

void CLICanClearInterruptDisableFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000100; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CLI;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1111011) == (CPUInit.PS & 0b1111011)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000100) == 0); //checking flag has been cleared
    TEST_CHECK(Cycles == CyclesExpected);
}

void CLVCanClearOverflowFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b01000000; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CLV;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b10111111) == (CPUInit.PS & 0b10111111)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b01000000) == 0); //checking flag has been cleared
    TEST_CHECK(Cycles == CyclesExpected);
}

void SECCanSetCarryFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000000; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SEC;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1111110) == (CPUInit.PS & 0b1111110)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000001) != 0); //checking flag has been set
    TEST_CHECK(Cycles == CyclesExpected);
}

void SEDCanSetDecimalModeFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000000; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SED;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1110111) == (CPUInit.PS & 0b1110111)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00001000) != 0); //checking flag has been set
    TEST_CHECK(Cycles == CyclesExpected);
}

void SEICanSetInterruptDisableFlag()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000000; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SEI;
    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK((cpu.PS & 0b1111011) == (CPUInit.PS & 0b1111011)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000100) != 0); //checking flag has been set
    TEST_CHECK(Cycles == CyclesExpected);
}

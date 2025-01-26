void TAXCanTransferPositiveARegToXReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.X = 0b00000000;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0b00110011); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TAXCanTransferZeroARegToXReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00000000;
    cpu.X = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0b00000000); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TAXCanTransferNegativeARegToXReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b11000000;
    cpu.X = 0b00000000;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0b11000000); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TAYCanTransferPositiveARegToYReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.Y = 0b00000000;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0b00110011); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TAYCanTransferZeroARegToYReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00000000;
    cpu.Y = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0b00000000); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TAYCanTransferNegativeARegToYReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b11000000;
    cpu.Y = 0b00000000;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TAY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0b11000000); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TXACanTransferPositiveXRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;

    cpu.Reset(bus);

    cpu.X = 0b00110011;
    cpu.A = 0b00000000;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TXA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00110011); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TXACanTransferZeroXRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0b00000000;
    cpu.A = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TXA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00000000); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TXACanTransferNegativeXRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0b11000000;
    cpu.A = 0b00000000;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TXA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b11000000); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TYACanTransferPositiveYRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0b00110011;
    cpu.A = 0b00000000;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TYA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00110011); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TYACanTransferZeroYRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0b00000000;
    cpu.A = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TYA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00000000); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void TYACanTransferNegativeYRegToAReg()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0b11000000;
    cpu.A = 0b00000000;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_TYA;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b11000000); //checking correct value in the A register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

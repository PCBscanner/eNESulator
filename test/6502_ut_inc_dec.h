void INXCanIncrementAZeroValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x00;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    TEST_CHECK(cpu.X == 0x01); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INXCanIncrementToZero() //wraps around without carry
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0xFF;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0x00); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INXCanIncrementToANegativeValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0xFE;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0xFF); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INYCanIncrementAZeroValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x00;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0x01); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INYCanIncrementToZero() //wraps around without carry
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0xFF;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0x00); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INYCanIncrementToANegativeValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0xFE;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_INY;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.Y == 0xFF); //checking correct value in the Y register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DEXCanDecrementANegativeValue() //wraps around
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x00;
    cpu.PS = 0b01111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_DEX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0xFF); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) != 0); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DEXCanDecrementToZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x01;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_DEX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0x00); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) != 0); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DEXCanDecrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x42;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_DEX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == 0x41); //checking correct value in the X register
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DECZPCanDecrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_DEC_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0x57;

    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x0042] == 0x56); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DECZPXCanDecrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x0F;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_DEC_ZPX;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0x004F] = 0x57;

    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x004F] == 0x56); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DECAbsCanDecrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_DEC_ABS;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0xFF02] = 0x30;
    bus.CPUMemory[0x3040] = 0x57;

    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x3040] == 0x56); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void DECAbsXCanDecrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x0F;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_DEC_ABSX;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0xFF02] = 0x30;
    bus.CPUMemory[0x304F] = 0x57;

    std::uint32_t CyclesExpected = 7;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x304F] == 0x56); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INCZPCanIncrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_INC_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0x57;

    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x0042] == 0x58); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INCZPXCanIncrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x0F;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_INC_ZPX;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0x004F] = 0x57;

    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x004F] == 0x58); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INCAbsCanIncrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_INC_ABS;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0xFF02] = 0x30;
    bus.CPUMemory[0x3040] = 0x57;

    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x3040] == 0x58); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void INCAbsXCanIncrementAPositiveValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x0F;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_INC_ABSX;
    bus.CPUMemory[0xFF01] = 0x40;
    bus.CPUMemory[0xFF02] = 0x30;
    bus.CPUMemory[0x304F] = 0x57;

    std::uint32_t CyclesExpected = 7;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x304F] == 0x58); //checking correct value in memory
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

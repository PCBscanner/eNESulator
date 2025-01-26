void ANDImmCanANDAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_IM;
    bus.CPUMemory[0xFFFD] = 0b10101010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00100010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ANDImmCanANDAZeroValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_IM;
    bus.CPUMemory[0xFFFD] = 0b00000000; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00000000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ANDImmCanANDToResultInANegativeValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10110011;
    cpu.PS = 0b01111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_IM;
    bus.CPUMemory[0xFFFD] = 0b11000000; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b10000000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORImmCanEORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10101010;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_IM;
    bus.CPUMemory[0xFFFD] = 0b11111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01010000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unmodified flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORImmCanEORAZeroValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b11111001;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_IM;
    bus.CPUMemory[0xFFFD] = 0b11111001; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00000000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORImmCanEORToResultInANegativeValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10110011;
    cpu.PS = 0b01111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_IM;
    bus.CPUMemory[0xFFFD] = 0b01110000; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b11000011); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAImmCanORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00101010;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_IM;
    bus.CPUMemory[0xFFFD] = 0b01111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01111010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //unmodified flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAImmCanORAZeroValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00000000;
    cpu.PS = 0b11111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_IM;
    bus.CPUMemory[0xFFFD] = 0b00000000; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00000000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAImmCanORAToResultInANegativeValue()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10110011;
    cpu.PS = 0b01111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_IM;
    bus.CPUMemory[0xFFFD] = 0b01110000; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b11110011); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ANDZPCanANDAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0b10101010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00100010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORZPCanEORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10101010;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0b11111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01010000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAZPCanORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00101010;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0b01111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01111010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ANDZPXCanANDAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.X = 0x16;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_ZPX;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0058] = 0b10101010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00100010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORZPXCanEORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10101010;
    cpu.X = 0x16;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_ZPX;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0058] = 0b11111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01010000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAZPXCanORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00101010;
    cpu.X = 0x16;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_ZPX;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0058] = 0b01111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01111010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ANDAbsCanANDAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_AND_ABS;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0xFFFE] = 0x58;
    bus.CPUMemory[0x5842] = 0b10101010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b00100010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void EORAbsCanEORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b10101010;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_EOR_ABS;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0xFFFE] = 0x58;
    bus.CPUMemory[0x5842] = 0b11111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01010000); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void ORAAbsCanORAValueFromMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00101010;
    cpu.X = 0x16;
    cpu.PS = 0b11111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_ORA_ABS;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0xFFFE] = 0x58;
    bus.CPUMemory[0x5842] = 0b01111010; //value we want to AND the accumulator with
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == 0b01111010); //checking correct value in accumulator
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting zero
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expecting a negative number
    TEST_CHECK(Cycles == CyclesExpected);
}

void BITZPCanUpdateStatusFlags01()
{
    //testing we can set N & V to 1 & 1, & Z to 0.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0xCC;
    cpu.PS = 0b00111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_BIT_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0xCC;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == CPUInit.A);
    TEST_CHECK((cpu.PS & 0b00111101) == (CPUInit.PS & 0b00111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting the zero flag to be set (A != 0)
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting the overflow flag to be set from memory
    TEST_CHECK((cpu.PS & 0b01000000) == 0b01000000); //expecting the overflow flag to be set from memory number
    TEST_CHECK(Cycles == CyclesExpected);
}

void BITZPCanUpdateStatusFlags02()
{
    //testing we can set N & V to 1 & 1, & Z to 1.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b00111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_BIT_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0b11001100;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == CPUInit.A);
    TEST_CHECK((cpu.PS & 0b00111101) == (CPUInit.PS & 0b00111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expecting the zero flag to be set (A != 0)
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting the overflow flag to be set from memory
    TEST_CHECK((cpu.PS & 0b01000000) == 0b01000000); //expecting the overflow flag to be set from memory number
    TEST_CHECK(Cycles == CyclesExpected);
}

void BITAbsCanUpdateStatusFlags01()
{
    //testing we can set N & V to 1 & 1, & Z to 0.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0xCC;
    cpu.PS = 0b00111111;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_BIT_ABS;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    bus.CPUMemory[0x8000] = 0xCC;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == CPUInit.A);
    TEST_CHECK((cpu.PS & 0b00111101) == (CPUInit.PS & 0b00111101));
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expecting the zero flag to be set (A != 0)
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting the overflow flag to be set from memory
    TEST_CHECK((cpu.PS & 0b01000000) == 0b01000000); //expecting the overflow flag to be set from memory number
    TEST_CHECK(Cycles == CyclesExpected);
}

void BITAbsCanUpdateStatusFlags02()
{
    //testing we can set N & V to 1 & 1, & Z to 1.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0b00110011;
    cpu.PS = 0b00111101;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_BIT_ABS;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    bus.CPUMemory[0x8000] = 0b11001100;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == CPUInit.A);
    TEST_CHECK((cpu.PS & 0b00111101) == (CPUInit.PS & 0b00111101)); //unaffected flags
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expecting the zero flag to be set (A != 0)
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expecting the overflow flag to be set from memory
    TEST_CHECK((cpu.PS & 0b01000000) == 0b01000000); //expecting the overflow flag to be set from memory number
    TEST_CHECK(Cycles == CyclesExpected);
}

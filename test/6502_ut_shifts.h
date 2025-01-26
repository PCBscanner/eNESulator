void ASLACCCanShiftTheValueOfZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100010;
    cpu.A = 0b00000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100010;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    // printf("cpu.PS: 0b%08b\n", cpu.PS);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLACCCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLACCCanShiftAPositiveValueToNegative()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b01000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b10000000;
    std::uint8_t PSExpected = 0b10100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);

}

void ASLACCCanShiftANegativeValueToCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0b10000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100001;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    // printf("cpu.PS: 0b%08b\n", cpu.PS);
    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLACCCanShiftANegativeValueToZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0b10000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    // printf("cpu.PS: 0b%08b\n", cpu.PS);
    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLZPCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 5;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0042] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLZPXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ZPX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0045] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0045] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLABSCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ABS;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3742] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3742] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ASLABSXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ASL_ABSX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3745] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 7;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3745] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

///////////////////////////////////////////////////////////////////////////////

void LSRACCCanShiftTheValueOfZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100010;
    cpu.A = 0b00000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100010;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    // printf("cpu.PS: 0b%08b\n", cpu.PS);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRACCCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRACCCanShiftANegativeValueToPositive()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b10000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b01000000;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);

}

void LSRACCCanShiftANegativeValueToCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0b10000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100001;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    // printf("cpu.PS: 0b%08b\n", cpu.PS);
    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRZPCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 5;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0042] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRZPXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ZPX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0045] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0045] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRABSCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ABS;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3742] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3742] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void LSRABSXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_LSR_ABSX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3745] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 7;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3745] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

///////////////////////////////////////////////////////

void ROLACCCanShiftTheValueOfOneWithoutCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLACCCanShiftTheValueOfOneWithCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000011;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLACCCanShiftAPositiveValueToNegative()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b01000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b10000000;
    std::uint8_t PSExpected = 0b10100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLACCCanShiftANegativeValueToCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b11000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b10000000;
    std::uint8_t PSExpected = 0b10100001;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLZPCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 5;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0042] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLZPXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ZPX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0045] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0045] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLABSCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ABS;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3742] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3742] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ROLABSXCanShiftTheValueOfOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROL_ABSX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3745] = 0b00000001;

    std::uint32_t CyclesExpected = 7 + 7;
    std::uint8_t AnswerExpected = 0b00000010;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3745] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}


///////////////////////////////////////////////////////

void RORACCCanShiftTheValueOfOneWithoutCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b00000000;
    std::uint8_t PSExpected = 0b00100011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void RORACCCanShiftTheValueOfOneWithCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100001;
    cpu.A = 0b00000001;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b10000000;
    std::uint8_t PSExpected = 0b10100001;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void RORACCCanShiftANegativeValueToPositive()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0b10000000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ACC;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0b01000000;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}


void RORZPCanShiftTheValueOfTwo()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110000;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0b00000010;

    std::uint32_t CyclesExpected = 7 + 5;
    std::uint8_t AnswerExpected = 0b00000001;
    std::uint8_t PSExpected = 0b00110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0042] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void RORZPXCanShiftTheValueOfTwo()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ZPX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0045] = 0b00000010;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000001;
    std::uint8_t PSExpected = 0b00110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x0045] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void RORABSCanShiftTheValueOfTwo()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ABS;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3742] = 0b00000010;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0b00000001;
    std::uint8_t PSExpected = 0b00110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3742] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void RORABSXCanShiftTheValueOfTwo()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110000;
    cpu.PC = 0xFF00;
    cpu.X = 0x03;
    bus.CPUMemory[0xFF00] = CPU::INS_ROR_ABSX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0xFF02] = 0x37;
    bus.CPUMemory[0x3745] = 0b00000010;

    std::uint32_t CyclesExpected = 7 + 7;
    std::uint8_t AnswerExpected = 0b00000001;
    std::uint8_t PSExpected = 0b00110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(bus.CPUMemory[0x3745] == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

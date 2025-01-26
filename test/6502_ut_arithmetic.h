void ADCFlagTest(std::uint32_t Cycles, CPU cpu, std::uint8_t AnswerExpected, std::uint8_t PSExpected, std::uint32_t CyclesExpected)
{
    TEST_CHECK(cpu.PS == PSExpected);
    TEST_CHECK(cpu.A == AnswerExpected);
    TEST_CHECK(Cycles == CyclesExpected);
}

void ADCImmWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_IMM;
    bus.CPUMemory[0xFF01] = 0x02;

    std::uint32_t CyclesExpected = 7 + 2;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCZPWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ZP;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0x02;

    std::uint32_t CyclesExpected = 7 + 3;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCZPXWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ZPX;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0045] = 0x02;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillAddZeroToZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100010;
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x00;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x00;
    std::uint8_t PSExpected = 0b00100010;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillAddZeroToZeroWithCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100011;
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x00;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x01;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillAddOneToFFAndSetCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0xFF;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x00;
    std::uint8_t PSExpected = 0b00100011; //we expect zero and a carry

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillSetNegativeFlagWhenTheResultIsNegative()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100010;
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x8D;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x8D;
    std::uint8_t PSExpected = 0b10100000; //we only expect negative

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithNegativeInputs()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100010;
    cpu.A = 0x80; //-128
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0xFF;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x7F;
    std::uint8_t PSExpected = 0b01100001; //we expect positive, overflow, carry

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithPositiveInputs()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100010;
    cpu.A = 0x7F; //+127
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01; //+1

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x80;
    std::uint8_t PSExpected = 0b11100000; //we expect negative, overflow, no carry

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsXWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABSX;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8003] = 0x02;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCAbsYWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.Y = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_ABSY;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8003] = 0x02;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCIndXWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_INDX;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0x0003] = 0x00;
    bus.CPUMemory[0x0004] = 0x80;
    bus.CPUMemory[0x8000] = 0x02;

    std::uint32_t CyclesExpected = 7 + 6;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void ADCIndYWillAddTwoPositiveNumbers()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00100000;
    cpu.A = 0x01;
    cpu.Y = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_ADC_INDY;
    bus.CPUMemory[0xFF01] = 0x42;
    bus.CPUMemory[0x0042] = 0x00;
    bus.CPUMemory[0x0043] = 0x80;
    bus.CPUMemory[0x8003] = 0x02;

    std::uint32_t CyclesExpected = 7 + 5;
    std::uint8_t AnswerExpected = 0x03;
    std::uint8_t PSExpected = 0b00100000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void SBCAbsWillSubtractZeroFromZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110011; //setting not of the carry bit
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x00;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x00;
    std::uint8_t PSExpected = 0b00110011;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    //printf("cpu.PS: 0b%08b\n", cpu.PS);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void SBCAbsWillSubtractOneFromZeroWithCarry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110010; //setting the not of the carry
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0xFE; //-2 expected
    std::uint8_t PSExpected = 0b10110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    //printf("cpu.PS: 0b%08b\n", cpu.PS);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void SBCAbsWillSubtractOneFromZero()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110011; //setting the not of the carry bit (assume no carry here, so set to 1)
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0xFF;
    std::uint8_t PSExpected = 0b10110000; //negative, no overflow (also means we DO NOT clear the carry flag).

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void SBCAbsWillSubtractOneFromOne()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110001; //setting the not of the carry bit (assume no carry here, so set to 1)
    cpu.A = 0x01;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01;

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x00;
    std::uint8_t PSExpected = 0b00110011; //zero, no overflow (also means we DO NOT clear the carry flag).

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);

}

void SBCAbsWillSubtractTwoNegativeNumbersAndGetSignedOverflow()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10110001;
    cpu.A = 0x80; //-128
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x01; //+1

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x7F; //real answer is -129, but we expect +127 as the answer produced
    std::uint8_t PSExpected = 0b01110001; //positive, overflow (also means we clear the carry flag).

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void SBCAbsWillSubtractAPositiveAndNegativeNumberAndGetSignedOverflow()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00110001;
    cpu.A = 0x7F; //+127
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_SBC_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0xFF; //-1

    std::uint32_t CyclesExpected = 7 + 4;
    std::uint8_t AnswerExpected = 0x80;
    std::uint8_t PSExpected = 0b11110000;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);
    ADCFlagTest(Cycles, cpu, AnswerExpected, PSExpected, CyclesExpected);
}

void CMPImmCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_IMM;
    bus.CPUMemory[0xFF01] = 0x42;

    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPImmCanCompareSmallPosAToLargePosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x01;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_IMM;
    bus.CPUMemory[0xFF01] = 0x30;

    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b10100000);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPImmCanCompareEqualValues()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x05;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_IMM;
    bus.CPUMemory[0xFF01] = 0x05;

    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100011);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPZPCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_ZP;
    bus.CPUMemory[0xFF01] = 0x22;
    bus.CPUMemory[0x0022] = 0x42;

    std::uint32_t CyclesExpected = 7 + 3;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPZPXCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_ZPX;
    bus.CPUMemory[0xFF01] = 0x22;
    bus.CPUMemory[0x0025] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPAbsCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPAbsXCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_ABSX;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8003] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPAbsYCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.Y = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_ABSY;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8003] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CMPIndXCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.X = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_INDX;
    bus.CPUMemory[0xFF01] = 0x04;
    bus.CPUMemory[0x0007] = 0x00;
    bus.CPUMemory[0x0008] = 0x80;
    bus.CPUMemory[0x8000] = 0x42;

    std::uint32_t CyclesExpected = 7 + 6;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}


void CMPIndYCanCompareLargePosAToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.A = 0x70;
    cpu.Y = 0x03;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CMP_INDY;
    bus.CPUMemory[0xFF01] = 0x04;
    bus.CPUMemory[0x0004] = 0x00;
    bus.CPUMemory[0x0005] = 0x80;
    bus.CPUMemory[0x8003] = 0x42;

    std::uint32_t CyclesExpected = 7 + 5;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPXImmCanCompareLargePosXToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.X = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPX_IMM;
    bus.CPUMemory[0xFF01] = 0x42;

    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPXZPCanCompareLargePosXToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.X = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPX_ZP;
    bus.CPUMemory[0xFF01] = 0x22;
    bus.CPUMemory[0x0022] = 0x42;

    std::uint32_t CyclesExpected = 7 + 3;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPXAbsCanCompareLargePosXToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.X = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPX_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPYImmCanCompareLargePosYToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.Y = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPY_IMM;
    bus.CPUMemory[0xFF01] = 0x42;

    std::uint32_t CyclesExpected = 7 + 2;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPYZPCanCompareLargePosYToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.Y = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPY_ZP;
    bus.CPUMemory[0xFF01] = 0x22;
    bus.CPUMemory[0x0022] = 0x42;

    std::uint32_t CyclesExpected = 7 + 3;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

void CPYAbsCanCompareLargePosYToSmallPosM()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b10100000;
    cpu.Y = 0x70;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_CPY_ABS;
    bus.CPUMemory[0xFF01] = 0x00;
    bus.CPUMemory[0xFF02] = 0x80;
    bus.CPUMemory[0x8000] = 0x42;

    std::uint32_t CyclesExpected = 7 + 4;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00100001);
    TEST_CHECK(Cycles == CyclesExpected);
}

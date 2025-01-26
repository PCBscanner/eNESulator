static void VerifyUnmodifiedStatusFlagsLD( const CPU& cpu, const CPU& CPUInit)
{
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101));
}

void LDAImmCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_IM;
    bus.CPUMemory[0xFFFD] = 0x84;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x84);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);

    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000);
    TEST_CHECK(Cycles == CyclesExpected);


}

void LDAImmCanLoadAZeroValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.A = 0x44; //spoofing a non-zero value to ensure it changes to zero
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_IM;
    bus.CPUMemory[0xFFFD] = 0x0;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x0);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;

}

void LDAZPCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0x37;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAZPXCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 5;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ZPX;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0047] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAZPXCanLoadAValueIntoARegrWhenItWraps()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x007F] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAAbsCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ABS;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4480] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAAbsXCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0x01;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ABSX;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4481] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAAbsXCanLoadAValueIntoARegrWhenItCrossesPageBndry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ABSX;
    bus.CPUMemory[0xFFFD] = 0x02;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4402+0x00FF & crosses page boundary
    bus.CPUMemory[0x4501] = 0x37;
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAAbsYCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0x01;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ABSY;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4481] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAAbsYCanLoadAValueIntoARegrWhenItCrossesPageBndry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_ABSY;
    bus.CPUMemory[0xFFFD] = 0x02;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4402+0xFF & crosses page boundary
    bus.CPUMemory[0x4501] = 0x37;
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAIndXCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0x04; //X is loaded with 0x04.
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_INDX; //initial instruction stored here.
    bus.CPUMemory[0xFFFD] = 0x02; //0x02 is stored in the accumulator.
    //we then want to add 0x02 and 0x04 to get 0x06. This is the location we shall look at next.
    bus.CPUMemory[0x0006] = 0x00; //0x0006 (= 0x06) from above contains 0x00.
    bus.CPUMemory[0x0007] = 0x80; //0x0007 (the next memory location where the PC increments to) contains 0x80. Thus, we want to read from memory location 0x8000..
    bus.CPUMemory[0x8000] = 0x37;
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAIndYCanLoadAValueIntoARegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0x04;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_INDY;
    bus.CPUMemory[0xFFFD] = 0x02; //we want to initially read from memory location 0x02 (= 0x0002)
    bus.CPUMemory[0x0002] = 0x00; //memory location 0x0002 has the value 0x00
    bus.CPUMemory[0x0003] = 0x80; //memory location 0x0003 has the value 0x80
    //INS_IND_Y should put 0x00 and 0x80 together to temporarily define the LSB of the 16-buit address (= 0x8000)
    //we then add Y to this value to give the 16-bit address of 0x8004.
    bus.CPUMemory[0x8004] = 0x37; //memory location 0x8004 has value 0x37
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDAIndYCanLoadAValueIntoARegrWhenItCrossesPageBndry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDA_INDY;
    bus.CPUMemory[0xFFFD] = 0x02; //we want to read from memory location 0x02 (= 0x0002)
    bus.CPUMemory[0x0002] = 0x02; //memory location 0x0002 has the value 0x02
    bus.CPUMemory[0x0003] = 0x80; //memory location 0x0003 has the value 0x80
    //INS_IND_Y should put 0x02 and 0x80 together to temporarily define the LSB of the 16-buit address (= 0x8002)
    //we then add Y (0x00FF) to this value to give the 16-bit address of 0x8101.
    bus.CPUMemory[0x8101] = 0x37; //memory location 0x8101 has value 0x37
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.A == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDXImmCanLoadAValueIntoXRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_IM;
    bus.CPUMemory[0xFFFD] = 0x84;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x84);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000);
    TEST_CHECK(Cycles == CyclesExpected);


}

void LDYImmCanLoadAValueIntoYRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_IM;
    bus.CPUMemory[0xFFFD] = 0x84;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x84);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000);
    TEST_CHECK(Cycles == CyclesExpected);


}

void LDXZPCanLoadAValueIntoXRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0x37;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYZPCanLoadAValueIntoYRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ZP;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0042] = 0x37;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDXZPYCanLoadAValueIntoXRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 5;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_ZPY;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0047] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYZPXCanLoadAValueIntoYRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 5;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ZPX;
    bus.CPUMemory[0xFFFD] = 0x42;
    bus.CPUMemory[0x0047] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYZPXCanLoadAValueIntoYRegrWhenItWraps()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x007F] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDXAbsCanLoadAValueIntoXRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_ABS;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4480] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYAbsCanLoadAValueIntoYRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ABS;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4480] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDXAbsYCanLoadAValueIntoXRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0x01;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_ABSY;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4481] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDXAbsYCanLoadAValueIntoXRegrWhenItCrossesPageBndry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.Y = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDX_ABSY;
    bus.CPUMemory[0xFFFD] = 0x02;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4402+0xFF & crosses page boundary
    bus.CPUMemory[0x4501] = 0x37;
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.X == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYAbsXCanLoadAValueIntoYRegr()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0x01;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ABSX;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4480 (little byte first)
    bus.CPUMemory[0x4481] = 0x37;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

void LDYAbsXCanLoadAValueIntoYRegrWhenItCrossesPageBndry()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    //given:
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_LDY_ABSX;
    bus.CPUMemory[0xFFFD] = 0x02;
    bus.CPUMemory[0xFFFE] = 0x44; //should load from 0x4402+0x00FF & crosses page boundary
    bus.CPUMemory[0x4501] = 0x37;
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    //when:
    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    //then:
    //registers
    TEST_CHECK(cpu.Y == 0x37);
    //flags
    VerifyUnmodifiedStatusFlagsLD(cpu, CPUInit);
    TEST_CHECK((cpu.PS & 0b00000010) == 0);
    TEST_CHECK((cpu.PS & 0b10000000) == 0);;
    TEST_CHECK(Cycles == CyclesExpected);

}

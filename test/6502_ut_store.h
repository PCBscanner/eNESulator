void STAZeroPageCanStoreARegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ZP;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x0080] = 0x00;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x0080] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAZeroPageXCanStoreARegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x2F;
    cpu.X = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    //address = 0x80 + 0x0F = 0x8F
    bus.CPUMemory[0x008F] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x008F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAZeroPageXCanStoreARegisterIntoMemoryWhenItWraps()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x2F;
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    //address = 0x80 + 0xFF = 0x7F (accounting for wrapping)
    bus.CPUMemory[0x007F] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x007F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STXZeroPageCanStoreXRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STX_ZP;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x0080] = 0x00;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x0080] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STXZeroPageYCanStoreXRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x2F;
    cpu.Y = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STX_ZPY;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x008F] = 0x00; //initialising as zero to make sure it changes
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x008F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STXZeroPageYCanStoreXRegisterIntoMemoryWhenItWraps()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x2F;
    cpu.Y = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STX_ZPY;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x007F] = 0x00; //initialising as zero to make sure it changes
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x007F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STYZeroPageCanStoreYRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STY_ZP;
    bus.CPUMemory[0xFFFD] = 0x80;
    bus.CPUMemory[0x0080] = 0x00;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x0080] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STYZeroPageXCanStoreYRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0x2F;
    cpu.X = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STY_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    //address = 0x80 + 0xFF = 0x8F
    bus.CPUMemory[0x008F] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x008F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STYZeroPageXCanStoreYRegisterIntoMemoryWhenItWraps()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0x2F;
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STY_ZPX;
    bus.CPUMemory[0xFFFD] = 0x80;
    //address = 0x80 + 0xFF = 0x7F (accounting for wrapping)
    bus.CPUMemory[0x007F] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x007F] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAAbsCanStoreARegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ABS;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    bus.CPUMemory[0x0080] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x8000] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STXAbsCanStoreXRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.X = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STX_ABS;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    bus.CPUMemory[0x0080] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x8000] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STYAbsCanStoreYRegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.Y = 0x2F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STY_ABS;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    bus.CPUMemory[0x8000] = 0x00;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x8000] == 0x2F);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAAbsXCanStoreARegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x42; //value to be stored
    cpu.X = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ABSX;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    //memory address location = 0x0080 + 0x0F = 0x008F
    bus.CPUMemory[0x800F] = 0x00; //initialising as zero
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x800F] == 0x42);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAAbsYCanStoreARegisterIntoMemory()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x42; //value to be stored
    cpu.Y = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_ABSY;
    bus.CPUMemory[0xFFFD] = 0x00;
    bus.CPUMemory[0xFFFE] = 0x80;
    //memory address location = 0x0080 + 0x0F = 0x008F
    bus.CPUMemory[0x800F] = 0x00; //initialising as zero
    std::uint32_t CyclesExpected = 5;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x800F] == 0x42);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAIndXCanStoreARegisterIntoMemory()
{
    ///For indirect indexed, the X register is added to the initial (zero page) address defined in the instruction (with wrap around) to give addr1.
    //We then look at the memory location defined by addr 1, read the LSB from here, and the MSB from the adjacent location. This then gives addr2.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x42; //value to be stored
    cpu.X = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_INDX;
    bus.CPUMemory[0xFFFD] = 0x20; //initial ZP address
    bus.CPUMemory[0x002F] = 0x00;
    bus.CPUMemory[0x0030] = 0x80;
    //memory address location = 0x8000
    bus.CPUMemory[0x8000] = 0x00; //initialising as zero
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x8000] == 0x42);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAIndXCanStoreARegisterIntoMemoryWhenItWraps()
{
    ///For indirect indexed, the X register is added to the initial ZP address (with wrap around) to give addr1.
    //We then look at the memory location defined by addr 1, read the LSB from here, and the MSB from the adjacent location. This then gives addr2.
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x42; //value to be stored
    cpu.X = 0xFF;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_INDX;
    bus.CPUMemory[0xFFFD] = 0x20; //initial ZP address
    //0x0020 + 0x00FF = 0x011F --> ZP address shall be 0x1F
    bus.CPUMemory[0x001F] = 0x00;
    bus.CPUMemory[0x0020] = 0x80;
    //memory address location = 0x8000
    bus.CPUMemory[0x8000] = 0x00; //initialising as zero
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x8000] == 0x42);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

void STAIndYCanStoreARegisterIntoMemory()
{
    //
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.A = 0x42; //value to be stored
    cpu.Y = 0x0F;
    cpu.PC = 0xFFFC;
    bus.CPUMemory[0xFFFC] = CPU::INS_STA_INDY;
    bus.CPUMemory[0xFFFD] = 0x20; //initial ZP address
    bus.CPUMemory[0x0020] = 0x00;
    bus.CPUMemory[0x0021] = 0x80;
    //memory address location = 0x8000
    //then we add 0x000F to it to get 0x800F as the location to point to
    bus.CPUMemory[0x800F] = 0x00; //initialising as zero
    std::uint32_t CyclesExpected = 6;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[0x800F] == 0x42);
    TEST_CHECK(cpu.PS == CPUInit.PS);

    TEST_CHECK(Cycles == CyclesExpected);
}

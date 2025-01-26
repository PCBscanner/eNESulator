void TSXCanTransferStackPointerToXRegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.X = 0x00;
    cpu.SP = 0x01;
    cpu.PS = 0b01111111; //forcing the PS into a state that shall change according to the PS
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_TSX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == cpu.SP);
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expexting a negative number
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expexting zero
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void TSXCanTransferAZeroStackPointerToXRegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.X = 0x01;
    cpu.SP = 0x00;
    cpu.PS = 0b11111111; //forcing the PS into a state that shall change according to the PS
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_TSX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == cpu.SP);
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expexting a negative number
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expexting zero
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void TSXCanTransferANegativeStackPointerToXRegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.X = 0x01;
    cpu.SP = 0xFF;
    cpu.PS = 0b01111111; //forcing the PS into a state that shall change according to the PS
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_TSX;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == cpu.SP);
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expexting a negative number
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expexting zero
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void TXSCanTransferXRegisterToStackPointer()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.X = 0xFF;
    cpu.SP = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_TXS;
    std::uint32_t CyclesExpected = 2;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.X == cpu.SP);
    TEST_CHECK(cpu.PS == CPUInit.PS); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void PHACanPushARegisterOntoTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.A = 0x42;
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_PHA;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)]);
    TEST_CHECK(cpu.PS == CPUInit.PS); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void PHPCanPushProcessorStatusOntoTheStack()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PS = 0b10000111; //setting an arbitrarily recognisable PS
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_PHP;
    std::uint32_t CyclesExpected = 3;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)] == 0b10110111);
    TEST_CHECK(cpu.PS == CPUInit.PS); //checking processor status has not been affected
    TEST_CHECK(Cycles == CyclesExpected);
}

void PLACanPullAValueFromStackToARegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PS = 0b11111111; //forcing the PS into a state that shall change according to the PS
    cpu.SP = 0xFE; //need to start at 0xFE because the value to be pulled is at 0xFF
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)] = 0x01; //this is the value that we wish to pull from the stack
    bus.CPUMemory[0xFF00] = CPU::INS_PLA;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == bus.CPUMemory[cpu.SPToAddr(0xFF)]); //checking A register has been set to the value in the stack
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expexting the negative flag to be set
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expexting zero flag to be set
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking all remaining flags are unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void PLACanPullAZeroValueFromStackToARegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PS = 0b11111101; //forcing the PS into a state that shall change according to the PS
    cpu.SP = 0xFE;
    cpu.A = 0x01;
    cpu.PC = 0xFF00;
    bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)] = 0x00;
    bus.CPUMemory[0xFF00] = CPU::INS_PLA;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == bus.CPUMemory[cpu.SPToAddr(0xFF)]); //checking A register has been set to the value in the stack
    TEST_CHECK((cpu.PS & 0b10000000) == 0); //not expexting a negative number
    TEST_CHECK((cpu.PS & 0b00000010) == 0b00000010); //expexting zero
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void PLACanPullANegativeValueFromStackToARegister()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PS = 0b01111111; //forcing the PS into a state that shall change according to the PS
    cpu.SP = 0xFE;
    cpu.A = 0x00;
    cpu.PC = 0xFF00;
    bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)] = 0xFF;
    bus.CPUMemory[0xFF00] = CPU::INS_PLA;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.A == bus.CPUMemory[cpu.SPToAddr(0xFF)]); //checking A register has been set to the value in the stack
    TEST_CHECK((cpu.PS & 0b10000000) == 0b10000000); //expexting a negative number
    TEST_CHECK((cpu.PS & 0b00000010) == 0); //not expexting zero
    TEST_CHECK((cpu.PS & 0b01111101) == (CPUInit.PS & 0b01111101)); //checking unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void PLPCanPullAValueFromStackToProcessorStatus()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);
    cpu.PS = 0b11111111;
    cpu.SP = 0xFE; //need to start at 0xFE because the value to be pulled is at 0xFF
    bus.CPUMemory[cpu.SPToAddr(cpu.SP+1)] = 0x00; //this is the value that we wish to pull from the stack
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_PLP;
    std::uint32_t CyclesExpected = 4;
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PS == 0b00110000);
    TEST_CHECK(Cycles == CyclesExpected);
}

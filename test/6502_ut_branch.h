void BEQCanBranchForwardsIfEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b00000010; //setting the zero flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BEQ;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BEQCanBranchForwardsAndCrossPageBoundaryIfEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b00000010; //setting the zero flag
    cpu.PC = 0xF0FA;
    bus.CPUMemory[0xF0FA] = CPU::INS_BEQ;
    bus.CPUMemory[0xF0FB] = 0x07; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 2; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xF103);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BEQDoesNotBranchIfNotEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111101; //clearing the zero flag
    cpu.PC = 0xF0FA;
    bus.CPUMemory[0xF0FA] = CPU::INS_BEQ;
    std::uint32_t CyclesExpected = 2; //2 to check if we need to branch, but no further branching required.
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xF0FC);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BEQCanBranchBackwardsIfEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b00000010; //setting the zero flag
    cpu.PC = 0xFFCC;
    bus.CPUMemory[0xFFCC] = CPU::INS_BEQ;
    bus.CPUMemory[0xFFCD] = 0xFC; //I want to move backwards by 4, which is 0xFC
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFFCA);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BNECanBranchForwardsIfNotEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111101; //clearing the zero flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BNE;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BNECanBranchForwardsAndCrossPageBoundaryIfNotEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111101; //clearing the zero flag
    cpu.PC = 0xF0FA;
    bus.CPUMemory[0xF0FA] = CPU::INS_BNE;
    bus.CPUMemory[0xF0FB] = 0x07; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 2; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xF103);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BNEDoesNotBranchIfEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b00000010; //setting the zero flag
    cpu.PC = 0xF0FA;
    bus.CPUMemory[0xF0FA] = CPU::INS_BNE;
    std::uint32_t CyclesExpected = 2; //2 to check if we need to branch, but no further branching required.
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xF0FC);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BNECanBranchBackwardsIfNotEqual()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111101; //clearing the zero flag
    cpu.PC = 0xFFCC;
    bus.CPUMemory[0xFFCC] = CPU::INS_BNE;
    bus.CPUMemory[0xFFCD] = 0xFC; //I want to move backwards by 4, which is 0xFC
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFFCA);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BCSCanBranchForwardsIfCarrySet()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b00000001; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BCS;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BCCCanBranchForwardsIfCarryClear()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b11111110; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BCC;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BMICanBranchForwardsIfNegativeSet()
{
    Mem mem;
    CPU cpu;
    Bus bus;

    cpu.Reset(bus);

    cpu.PS = 0b10000000; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BMI;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BPLCanBranchForwardsIfNegativeClear()
{
    Mem mem;
    CPU cpu;
    Bus bus;

    std::uint32_t Cycles = cpu.Reset(bus);

    cpu.PS = 0b01111111; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BPL;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BVSCanBranchForwardsIfOverflowSet()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b01000000; //setting the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BVS;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

void BVCCanBranchForwardsIfOverflowClear()
{
    Mem mem;
    CPU cpu;
    Bus bus;
    cpu.Reset(bus);

    cpu.PS = 0b10111111; //clearing the flag
    cpu.PC = 0xFF00;
    bus.CPUMemory[0xFF00] = CPU::INS_BVC;
    bus.CPUMemory[0xFF01] = 0x03; //positive branch displacement
    std::uint32_t CyclesExpected = 2 + 1; //2 to check if we need to branch, +1 if branch is successful, no new page
    std::uint32_t Cycles = 0;

    CPU CPUInit = cpu;
    cpu.Execute(Cycles, bus);

    TEST_CHECK(cpu.PC == 0xFF05);
    TEST_CHECK(cpu.PS == CPUInit.PS); //unaffected flags
    TEST_CHECK(Cycles == CyclesExpected);
}

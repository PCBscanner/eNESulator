#include "../src/6502.h"

//RESETTING THE CPU
std::uint32_t CPU::Reset(Bus& bus) //returning the number of cycles used on a reset
{
    PC = 0xFFFC; //6502 reads the reset vector from 0xFFFC and 0xFFFD
    SP -= 3;
    PS |= 0b00100100; //TBC what this should actually be
    A = X = Y = 0x00;
    std::uint32_t Cycles = 0;
    PC = FetchWord(Cycles, bus);
    return Cycles += 5; //total number of cycles used for the reset is 7
}

//FETCHING, READING, AND WRITING, FROM MEMORY
std::uint8_t CPU::FetchByte(std::uint32_t& Cycles, Bus& bus) // reading from where the program counter is right now and then incrementing it
{
    std::uint8_t Data = bus.ReadCPUBus(PC);
    PC++; //incrementing the program counter
    Cycles++;
    return Data;
}

std::uint16_t CPU::FetchWord(std::uint32_t& Cycles, Bus& bus) // reading from where the program counter is right now and then incrementing it (twice)
{
    //6502 is little endian
    //first byte is LSB
    std::uint8_t LoByte = FetchByte(Cycles, bus);
    std::uint8_t HiByte = FetchByte(Cycles, bus);
    return LoByte | (HiByte << 8); //bit-shifting and using logical OR to get the 16-bit (2 byte) word.
}

std::uint8_t CPU::ReadByte(std::uint32_t& Cycles, std::uint16_t Address, Bus& bus) //reading an 8-bit from a given 16-bit memory address.
//note that we do not increment the program counter for a read byte. Why?
{
    std::uint8_t Data = bus.ReadCPUBus(Address);
    Cycles++;
    return Data;
}

std::uint16_t CPU::ReadWord(std::uint32_t& Cycles, std::uint16_t Address, Bus& bus) //reading a 16-bit value from a given 16-bit memory address.
//note that we do not increment the program counter for a read byte
{
    std::uint8_t LoByte = ReadByte (Cycles, Address, bus);
    std::uint8_t HiByte = ReadByte (Cycles, Address+1, bus);
    return LoByte | (HiByte << 8);
}

//write 1 byte to memory
void CPU::WriteByte(std::uint8_t Value, std::uint32_t& Cycles, std::uint16_t Address, Bus& bus)
{
    bus.WriteCPUBus(Value, Address);
    Cycles++;
}

//write 2 bytes to memory
void CPU::WriteWord( std::uint16_t Value, std::uint32_t& Cycles, std::uint16_t Address, Bus& bus)
{
    bus.WriteCPUBus((Value & 0xFF), Address);
    Cycles++;
    bus.WriteCPUBus((Value >> 8), Address+1);
    Cycles --;
}

std::uint16_t CPU::SPToAddr(std::uint8_t SP) const
{
    return 0x0100 | SP;
}

//push the program counter to the stack
void CPU::PushPCToStack(std::uint32_t& Cycles, Bus& bus, std::uint32_t Offset)
{
    WriteByte((PC+Offset & 0xFF00)>>8, Cycles, SPToAddr(SP), bus); //writing the HiByte first
    SP--; //decrementing the SP after we push
    WriteByte((PC+Offset & 0x00FF), Cycles, SPToAddr(SP), bus); //writing the LoByte
    SP--; //decrementing the SP after we push
    Cycles++; //Is this extra cycle correct here?
}

//pushing a single byte to the stack
void CPU::PushByteOntoStack(std::uint8_t Value, std::uint32_t& Cycles, Bus& bus)
{
    WriteByte(Value, Cycles, SPToAddr(SP), bus);
    SP--; //decrementing the SP after we push
    Cycles++;
}

std::uint8_t CPU::PullByteFromStack(std::uint32_t& Cycles, Bus& bus)
{
    SP++; //incrementing the SP before we pull
    Cycles++;
    std::uint8_t Value = ReadByte(Cycles, SPToAddr(SP), bus);
    return Value;
}

std::uint16_t CPU::PullWordFromStack(std::uint32_t& Cycles, Bus& bus)
{
    SP++; //incrementing the SP before we pull
    Cycles++;
    std::uint8_t LoByte = ReadByte(Cycles, SPToAddr(SP), bus);
    SP++; //incrementing the SP before we pull
    Cycles++;
    std::uint8_t HiByte = ReadByte(Cycles, SPToAddr(SP), bus);
    return LoByte | (HiByte << 8);;
}

//SETTING PROCESSOR STATUS FLAGS
//sets the correct processor status after loading a value
void CPU::LDValueSetProcessorStatus(std::uint8_t Value)
{
    if (Value == 0)
    {
        PS |= 0b00000010; //setting Z = 1
    }
    else
    {
        PS &= 0b11111101; //setting Z = 0;
    }
    if ((Value & 0b10000000) > 0) //checking if 7th bit of the register is 0
    {
        PS |= 0b10000000; //setting N = 1
    }
    else
    {
        PS &= 0b01111111; //setting N = 0;
    }
}

void CPU::SetStatusFlagsADC(std::uint8_t A, std::uint8_t AInit, std::uint8_t Operand, std::uint16_t AWord)
{
    //negative flag
    if ( (A & 0b10000000) == 0 )
    {
        PS &= 0b01111111;
    }
    else
    {
        PS |= 0b10000000;
    }
    //zero flag
    if ( A != 0 )
    {
        PS &= 0b11111101;
    }
    else
    {
        PS |= 0b00000010;
    }
    //carry flag
    if ((AWord & 0xFF00) != 0) //if we have overflowed
    {
        PS |= 0b00000001; //setting the flag
    }
    else
    {
        PS &= 0b11111110; //clearing the flag
    }
    //overflow flag
    if (((AInit < 0x80) && (Operand < 0x80) && (A >= 0x80)) || ((AInit >= 0x80) && (Operand >= 0x80) && (A < 0x80)))
    {
        PS |= 0b01000000;
    }
    else
    {
        PS &= 0b10111111;
    }
}

void CPU::SetStatusFlagsSBC(std::uint8_t A, std::uint8_t AInit, std::uint8_t Operand, std::uint16_t AWord)
{
    //negative flag
    if ( (A & 0b10000000) == 0 )
    {
        PS &= 0b01111111;
    }
    else
    {
        PS |= 0b10000000;
    }
    //zero flag
    if ( A != 0 )
    {
        PS &= 0b11111101;
    }
    else
    {
        PS |= 0b00000010;
    }
    //carry flag
    if ((AWord & 0xFF00) != 0) //if we have overflowed
    {
        PS &= 0b11111110;
    }
    else
    {
        PS |= 0b00000001;
    }
    //overflow flag
    if (((AInit < 0x80) && (Operand >= 0x80) && (A >= 0x80)) || ((AInit >= 0x80) && (Operand < 0x80) && (A < 0x80)))
    {
        PS |= 0b01000000;
    }
    else
    {
        PS &= 0b10111111;
    }
}

void CPU::SetStatusFlagsCompare(std::uint8_t Register, std::uint8_t Operand)
{
    //carry flag
    if(Register >= Operand)
    {
        PS |= 0b00000001;
    }
    else
    {
        PS &= 0b11111110;
    }
    //zero flag
    if(Register == Operand)
    {
        PS |= 0b00000010;
    }
    else
    {
        PS &= 0b11111101;
    }
    //negative flag
    if( ( (Register - Operand) & 0b10000000 ) != 0 )
    {
        PS |= 0b10000000;
    }
    else
    {
        PS &= 0b01111111;
    }
}

void CPU::SetStatusFlagsShiftLeft(std::uint16_t ValueShifted)
{
    //carry flag
    if ((ValueShifted & 0b100000000) == 0) //old bit 7 was not set
    {
        PS &= 0b11111110; //clear flag
    }
    else
    {
        PS |= 0b00000001; //setting flag
    }
    //zero flag
    if ((ValueShifted & 0xFF) != 0)
    {
        PS &= 0b11111101; //clear flag
    }
    else
    {
        PS |= 0b00000010; //setting flag
    }
    //negative flag
    if ((ValueShifted & 0b10000000) == 0) //new bit 7 is not set --> positive
    {
        PS &= 0b01111111; //clear flag
    }
    else
    {
        PS |= 0b10000000; //setting flag
    }
}

void CPU::SetStatusFlagsShiftRight(std::uint8_t ValueShifted, std::uint8_t Value)
{
    //carry flag
    if ((Value & 0b00000001) == 0) //old bit 0 was not set
    {
        PS &= 0b11111110; //clear flag
    }
    else
    {
        PS |= 0b00000001; //setting flag
    }
    //zero flag
    if ((ValueShifted & 0xFF) != 0)
    {
        PS &= 0b11111101; //clear flag
    }
    else
    {
        PS |= 0b00000010; //setting flag
    }
    //negative flag
    if ((ValueShifted & 0b10000000) == 0) //new bit 7 is not set --> positive
    {
        PS &= 0b01111111; //clear flag
    }
    else
    {
        PS |= 0b10000000; //setting flag
    }
}

void CPU::SetStatusFlagsRotateLeft(std::uint16_t ValueShifted)
{
    //carry flag
    if ((ValueShifted & 0b100000000) == 0) //old bit 7 was not set
    {
        PS &= 0b11111110; //clear flag
    }
    else
    {
        PS |= 0b00000001; //setting flag
    }
    //zero flag
    if ((ValueShifted & 0xFF) != 0)
    {
        PS &= 0b11111101; //clear flag
    }
    else
    {
        PS |= 0b00000010; //setting flag
    }
    //negative flag
    if ((ValueShifted & 0b10000000) == 0) //new bit 7 is not set --> positive
    {
        PS &= 0b01111111; //clear flag
    }
    else
    {
        PS |= 0b10000000; //setting flag
    }
}

void CPU::SetStatusFlagsRotateRight(std::uint8_t ValueShifted, std::uint8_t Value)
{
    //carry flag
    if ((Value & 0b00000001) == 0) //old bit 0 was not set
    {
        PS &= 0b11111110; //clear flag
    }
    else
    {
        PS |= 0b00000001; //setting flag
    }
    //zero flag
    if ((ValueShifted & 0xFF) != 0)
    {
        PS &= 0b11111101; //clear flag
    }
    else
    {
        PS |= 0b00000010; //setting flag
    }
    //negative flag
    if ((ValueShifted & 0b10000000) == 0) //new bit 7 is not set --> positive
    {
        PS &= 0b01111111; //clear flag
    }
    else
    {
        PS |= 0b10000000; //setting flag
    }
}

//ARITHMETIC OPERATIONS
void CPU::CalculateADC(std::uint8_t Operand)
{
    std::uint32_t CarryFlag = (PS & 0b00000001);
    if ((PS & 0b00001000) != 0)
    {
        printf("Decimal mode not yet implemented.\n");
        // throw -1;
    }
    std::uint16_t AWord = A; //initially converting to 16-bit
    std::uint8_t AInit = A; //used for determining the overflow flag
    AWord += Operand;
    AWord += CarryFlag;
    A = ( AWord & 0xFF ); //converting back to an 8-bit number
    SetStatusFlagsADC(A, AInit, Operand, AWord);
}

void CPU::CalculateSBC(std::uint8_t Operand)
{
    std::uint32_t CarryFlag = (PS & 0b00000001);
    if ((PS & 0b00001000) != 0)
    {
        printf("Decimal mode not yet implemented.\n");
        // throw -1;
    }
    std::uint16_t AWord = A; //initially converting to 16-bit
    std::uint8_t AInit = A; //used for determining the overflow flag
    AWord += ((~Operand));
    AWord += CarryFlag;
    A = ( AWord & 0xFF ); //converting back to an 8-bit number
    SetStatusFlagsSBC(A, AInit, Operand, AWord);
}

//SHIFT OPERATIONS
void CPU::RotateLeft(std::uint8_t Value, std::uint16_t  Addr, std::uint32_t& Cycles, Bus& bus, std::string AddrMode)
{
    std::uint16_t  Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
    Temp += (PS & 0b00000001); //setting bit 0 to the original carry bit
    SetStatusFlagsRotateLeft(Temp);
    if (AddrMode == "A") //Accumulator addressing mode
    {
        A = (Temp & 0xFF);  //extracting the LSB from the word
    }
    else
    {
        WriteByte((Temp & 0xFF), Cycles, Addr, bus);
    }
    Cycles++;
}

void CPU::RotateRight(std::uint8_t Value, std::uint16_t  Addr, std::uint32_t& Cycles, Bus& bus, std::string AddrMode)
{
    std::uint8_t Temp = Value >> 1;
    Temp += ((PS & 0b00000001) << 7); //adding the carry bit
    SetStatusFlagsRotateRight(Temp, Value);
    if (AddrMode == "A")
    {
        A = Temp;
    }
    else
    {
        WriteByte(Temp, Cycles, Addr, bus);
    }
    Cycles++;
}

//ADDRESSING MODES
std::uint16_t CPU::AddrZeroPage(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus);
    return ZeroPageAddr;
}

std::uint16_t CPU::AddrZeroPageX(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus);
    ZeroPageAddr += X; //wrap account for in the variable type
    Cycles++;
    return ZeroPageAddr;
}

std::uint16_t CPU::AddrZeroPageY(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus);
    ZeroPageAddr += Y; //wrap account for in the variable type
    Cycles++;
    return ZeroPageAddr;
}

std::uint16_t CPU::AddrAbs(std::uint32_t& Cycles, Bus& bus)
{
    std::uint16_t  AbsAddr = FetchWord(Cycles, bus);
    return AbsAddr;
}

std::uint16_t CPU::AddrAbsX(std::uint32_t& Cycles, Bus& bus)
{
    std::uint16_t  AbsAddr = FetchWord(Cycles, bus); //2 cycles to fetch the word
    std::uint16_t  AbsAddrX = AbsAddr + X; //WHY DOES THIS NOT COST A CYCLE?
    //checking if the page boundary has been crossed
    //the operand's low byte is fetched before the high byte, so the processor can start adding the X register's value before it has the high byte. If there is no carry operation, the entire indexed operation takes only four clocks,
    if ( ( (AbsAddr & 0xFF) + X) > 0xFF)
    {
        Cycles++;
    }
    return AbsAddrX;
}

std::uint16_t CPU::AddrAbsXWrite(std::uint32_t& Cycles, Bus& bus) //used for instructions that always use an additional cycle regardless of if a page boundary has been crossed
{
    std::uint16_t  AbsAddr = FetchWord(Cycles, bus); //2 cycles to fetch the word
    std::uint16_t  AbsAddrX = AbsAddr + X;
    Cycles++;
    return AbsAddrX;
}

std::uint16_t CPU::AddrAbsY(std::uint32_t& Cycles, Bus& bus)
{
    std::uint16_t  AbsAddr = FetchWord(Cycles, bus); //2 cycles to fetch the word
    std::uint16_t  AbsAddrY = AbsAddr + Y; //WHY DOES THIS NOT COST A CYCLE?
    //checking if the page boundary has been crossed
    //the operand's low byte is fetched before the high byte, so the processor can start adding the X register's value before it has the high byte. If there is no carry operation, the entire indexed operation takes only four clocks,
    if ( ( (AbsAddr & 0xFF) + Y) > 0xFF)
    {
        Cycles++;
    }
    return AbsAddrY;
}

std::uint16_t CPU::AddrAbsYSTA(std::uint32_t& Cycles, Bus& bus)
{
    std::uint16_t  AbsAddr = FetchWord(Cycles, bus); //2 cycles to fetch the word
    std::uint16_t  AbsAddrY = AbsAddr + Y;
    Cycles++;
    return AbsAddrY;
}

std::uint16_t CPU::AddrIndX(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus);
    std::uint8_t AddrTmp = ((ZeroPageAddr + X)&0xFF);
    Cycles++;
    std::uint8_t LoByte = ReadByte(Cycles, AddrTmp, bus);
    AddrTmp++;
    std::uint8_t HiByte = ReadByte(Cycles, (AddrTmp&0xFF), bus);
    std::uint16_t  AddrIndX = LoByte | (HiByte << 8);
    return AddrIndX;
}

std::uint16_t CPU::AddrIndY(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus);
    std::uint8_t LoByte = ReadByte(Cycles, ZeroPageAddr, bus);
    std::uint8_t HiByte = ReadByte(Cycles, ((ZeroPageAddr+1)&0xFF), bus);
    std::uint16_t  AddrTmp = LoByte | (HiByte << 8);
    std::uint16_t  AddrIndY = AddrTmp + Y;
    if ( ( ( AddrTmp & 0xFF ) + Y ) > 0xFF )
    {
        Cycles++;
    }
    return AddrIndY;
}

//adding a separate IndY addressing mode for STA because we always consume 6 cycles regardless of if we cross a page boundary
std::uint16_t CPU::AddrIndYSTA(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus);
    std::uint8_t LoByte = ReadByte(Cycles, ZeroPageAddr, bus);
    std::uint8_t HiByte = ReadByte(Cycles, ((ZeroPageAddr+1)&0xFF), bus);
    std::uint16_t  AddrTmp = LoByte | (HiByte << 8);
    std::uint16_t  AddrIndY = AddrTmp + Y;
    Cycles++;
    return AddrIndY;
}

//LOADING REGISTERS
//loading a register with the value from the location specified in the PC
void CPU::LoadRegFromPC(std::uint8_t& Register, std::uint32_t& Cycles, Bus& bus)
{
    Register = FetchByte(Cycles, bus);
    LDValueSetProcessorStatus(Register);
}

//loading a register with the value from the location specified by an address
void CPU::LoadRegFromAddr(std::uint8_t& Register, std::uint32_t& Cycles, std::uint16_t Address, Bus& bus)
{
    Register = ReadByte(Cycles, Address, bus);
    LDValueSetProcessorStatus(Register);
}

//BRANCHING
void CPU::BranchToLocn(std::int8_t Displacement, std::uint32_t& Cycles, Bus& bus)
{
    PC = PC + Displacement;
    Cycles++;
    if ( ( ((PC-Displacement) & 0x00FF) + Displacement) > 0x00FF ) //checking if we crossed into a new page
    {
        Cycles++;
    }
}

//ACTIVATING THE NMI INSTRUCTION
void CPU::ExecuteNMI(std::uint32_t& Cycles, Bus& bus)
{
    PushPCToStack(Cycles, bus, 0);
    PushByteOntoStack(PS, Cycles, bus);
    PC = ReadWord(Cycles, 0xFFFA, bus);
}

//EXECUTING INSTRUCTIONS
void CPU::Execute(std::uint32_t& Cycles, Bus& bus)
{
    std::uint8_t Ins = FetchByte(Cycles, bus);
    switch ( Ins )
    {
        case INS_LDA_IM:
        {
            LoadRegFromPC(A, Cycles, bus);
        } break;

        case INS_LDX_IM:
        {
            LoadRegFromPC(X, Cycles, bus);
        } break;

        case INS_LDY_IM:
        {
            LoadRegFromPC(Y, Cycles, bus);
        } break;

        case INS_LDA_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        } break;

        case INS_LDX_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            LoadRegFromAddr(X, Cycles, Addr, bus);
        } break;

        case INS_LDY_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            LoadRegFromAddr(Y, Cycles, Addr, bus);
        } break;

        case INS_LDA_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);;
            LoadRegFromAddr(A, Cycles, Addr, bus);
        } break;

        case INS_LDY_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);;
            LoadRegFromAddr(Y, Cycles, Addr, bus);
        } break;

        case INS_LDA_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            // printf("Addr: 0x%04x\n", Addr);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        } break;

        case INS_LDX_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            LoadRegFromAddr(X, Cycles, Addr, bus);
        } break;

        case INS_LDY_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            LoadRegFromAddr(Y, Cycles, Addr, bus);
        } break;

        case INS_LDA_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        } break;

        case INS_LDY_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            LoadRegFromAddr(Y, Cycles, Addr, bus);
        } break;

        case INS_LDA_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        } break;

        case INS_LDX_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            LoadRegFromAddr(X, Cycles, Addr, bus);
        } break;

        case INS_LDX_ZPY:
        {
            std::uint16_t Addr = AddrZeroPageY(Cycles, bus);;
            LoadRegFromAddr(X, Cycles, Addr, bus);
        } break;

        case INS_LDA_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        }break;

        case INS_LDA_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            LoadRegFromAddr(A, Cycles, Addr, bus);
        }break;

        case INS_STA_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        } break;

        case INS_STX_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            WriteByte(X, Cycles, Addr, bus);
        } break;

        case INS_STY_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            WriteByte(Y, Cycles, Addr, bus);
        } break;

        case INS_STA_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        } break;

        case INS_STX_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            WriteByte(X, Cycles, Addr, bus);
        } break;

        case INS_STY_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            WriteByte(Y, Cycles, Addr, bus);
        } break;

        case INS_STA_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        }break;

        case INS_STY_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            WriteByte(Y, Cycles, Addr, bus);
        }break;

        case INS_STA_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        }break;

        case INS_STA_ABSY:
        {
            std::uint16_t Addr = AddrAbsYSTA(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        }break;

        case INS_STA_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        }break;

        case INS_STA_INDY:
        {
            std::uint16_t Addr = AddrIndYSTA(Cycles, bus);
            WriteByte(A, Cycles, Addr, bus);
        }break;

        case INS_STX_ZPY:
        {
            std::uint16_t Addr = AddrZeroPageY(Cycles, bus);
            WriteByte(X, Cycles, Addr, bus);
        }break;


        case INS_JSR:
        {
            std::uint16_t PCInit = PC; //taking a copy of the program counter
            std::uint8_t LoByte = ReadByte(Cycles, PCInit, bus); //fetching the low byte of the address
            PushPCToStack(Cycles, bus, 1);
            std::uint8_t HiByte = ReadByte(Cycles, PCInit+1, bus); //fetching the high byte of the address AFTER pushing PC to stack.
            PC = LoByte | (HiByte << 8);
        }break;

        case INS_RTS:
        {
            std::uint16_t ReturnAddr = PullWordFromStack(Cycles, bus);
            PC = ReturnAddr + 1; //the return address points at the last byte of the previous instruction, therefore we add 1.
            Cycles++;
        }break;

        case INS_JMP_ABS:
        {
            std::uint16_t Addr = FetchWord(Cycles, bus);
            PC = Addr;
        } break;

        case INS_JMP_IND:
        {
            //workaround for incorrect operation of the original 6502
            //An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page.
            std::uint16_t  AddrIndirect = FetchWord(Cycles, bus);
            std::uint8_t LoByte = ReadByte(Cycles, AddrIndirect, bus);
            std::uint8_t HiByte = 0;
            if ((AddrIndirect & 0xFF) == 0xFF)
            {
                HiByte = ReadByte(Cycles, (AddrIndirect-0xFF), bus);
            }
            else
            {
                HiByte = ReadByte(Cycles, (AddrIndirect+1), bus);
            }
            PC = LoByte | (HiByte << 8);
        }break;

        case INS_TSX:
        {
            X = SP;
            Cycles++;
            LDValueSetProcessorStatus(X);
        }break;

        case INS_TXS:
        {
            SP = X;
            Cycles++;
        }break;

        case INS_PHA:
        {
            PushByteOntoStack(A, Cycles, bus);
        }break;

        case INS_PHP:
        {
            //bits 4 and 5 are always set when we push to the stack.
            PushByteOntoStack((PS | 0b00110000), Cycles, bus);
        }break;

        case INS_PLA:
        {
            A = PullByteFromStack(Cycles, bus);
            Cycles++;
            LDValueSetProcessorStatus(A);
        }break;

        case INS_PLP:
        {
            //bits 4 and 5 are unaffected by PLP. We only want to update bits 0-3 and 6-7 from the stack.
            PS = (PS & 0b00110000) | (PullByteFromStack(Cycles, bus) & 0b11001111);
            Cycles++;
        }break;

        case INS_AND_IM:
        {
            A &= FetchByte(Cycles, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_IM:
        {
            A ^= FetchByte(Cycles, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_IM:
        {
            A |= FetchByte(Cycles, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_AND_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            A &= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_EOR_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            A ^= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_ORA_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            A |= ReadByte(Cycles, Addr, bus);
            LDValueSetProcessorStatus(A);
        }break;

        case INS_BIT_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //reading the value from memory
            if ((A & M) != 0)
            {
                PS &= 0b11111101; //setting Z flag to 0
            }
            else
            {
                PS |= 0b00000010; //setting Z flag to 1
            }
            PS &= 0b00111111; //initially clearing bits 6 & 7 for the next operation
            PS |= (M & 0b11000000); //setting bits 6 & 7 from the value from memory
        }break;

        case INS_BIT_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //reading the value from memory
            if ((A & M) != 0)
            {
                PS &= 0b11111101; //setting Z flag to 0
            }
            else
            {
                PS |= 0b00000010; //setting Z flag to 1
            }
            PS &= 0b00111111; //initially clearing bits 6 & 7 for the next operation
            PS |= (M & 0b11000000); //setting bits 6 & 7 from the value from memory
        }break;

        case INS_TAX:
        {
            X = A;
            Cycles++;
            LDValueSetProcessorStatus(X);
        }break;

        case INS_TAY:
        {
            Y = A;
            Cycles++;
            LDValueSetProcessorStatus(Y);
        }break;

        case INS_TXA:
        {
            A = X;
            Cycles++;
            LDValueSetProcessorStatus(A);
        }break;

        case INS_TYA:
        {
            A = Y;
            Cycles++;
            LDValueSetProcessorStatus(A);
        }break;

        case INS_INX:
        {
            X++;
            Cycles++;
            LDValueSetProcessorStatus(X);
        }break;

        case INS_INY:
        {
            Y++;
            Cycles++;
            LDValueSetProcessorStatus(Y);
        }break;

        case INS_DEX:
        {
            X--;
            Cycles++;
            LDValueSetProcessorStatus(X);
        }break;

        case INS_DEY:
        {
            Y--;
            Cycles++;
            LDValueSetProcessorStatus(Y);
        }break;

        case INS_INC_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value++;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_INC_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value++;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_INC_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value++;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_INC_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value++;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_DEC_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value--;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_DEC_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value--;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_DEC_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value--;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_DEC_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //1 cycle
            std::uint8_t Value = ReadByte(Cycles, Addr, bus);
            Value--;
            Cycles++;
            WriteByte(Value, Cycles, Addr, bus);
            LDValueSetProcessorStatus(bus.ReadCPUBus(Addr));
        }break;

        case INS_BEQ:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b00000010) != 0) //if the zero flag is set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BNE:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b00000010) == 0) //if the zero flag is not set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BCS:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b00000001) != 0) //if the zero flag is set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BCC:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b00000001) == 0) //if the zero flag is not set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;


        case INS_BMI:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b10000000) != 0) //if the flag is set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BPL:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b10000000) == 0) //if the flag is not set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BVS:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b01000000) != 0) //if the flag is set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_BVC:
        {
            std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus));
            if ((PS & 0b01000000) == 0) //if the flag is not set, then we branch
            {
                BranchToLocn(Displacement, Cycles, bus);
            }
        }break;

        case INS_CLC:
        {
            PS &= 0b11111110;
            Cycles++;
        }break;

        case INS_CLD:
        {
            PS &= 0b11110111;
            Cycles++;
        }break;

        case INS_CLI:
        {
            PS &= 0b11111011;
            Cycles++;
        }break;

        case INS_CLV:
        {
            PS &= 0b10111111;
            Cycles++;
        }break;

        case INS_SEC:
        {
            PS |= 0b00000001;
            Cycles++;
        }break;

        case INS_SED:
        {
            PS |= 0b00001000;
            Cycles++;
        }break;

        case INS_SEI:
        {
            PS |= 0b00000100;
            Cycles++;
        }break;

        case INS_BRK:
        {
            PushPCToStack(Cycles, bus, +1); //3 cycles
            PushByteOntoStack((PS | 0b00010000), Cycles, bus); //set bit 4 on the PS we push to the stack (do not update actual PS)
            PC = ReadWord(Cycles, 0xFFFE, bus); //2 cycles
            PS |= 0b00000100; //setting bit 2 (I flag)
            Cycles--; //unsure why this is required?
        }break;

        case INS_RTI:
        {
            std::uint8_t PSFromStack = PullByteFromStack(Cycles, bus);
            PSFromStack &= 0b11101111; //clearing bit 4 from the PS we pulled from the stack
            PS &= 0b00100000; //retaining bit 5 from the original PS because it is unaffected by this operation
            PS |= PSFromStack;
            PC = PullWordFromStack(Cycles, bus);
            Cycles--; //unsure why this is required?
        }break;

        case INS_NOP:
        {
            Cycles++;
        }break;

        case INS_ADC_IMM:
        {
            std::uint8_t Operand = FetchByte(Cycles, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;

        case INS_ADC_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateADC(Operand);
        }break;
        ///////////////

        case INS_SBC_IMM:
        {
            std::uint8_t Operand = FetchByte(Cycles, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        case INS_SBC_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            CalculateSBC(Operand);
        }break;

        //////////////////////////

        case INS_CMP_IMM:
        {
            std::uint8_t Operand = FetchByte(Cycles, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_ABSX:
        {
            std::uint16_t Addr = AddrAbsX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_ABSY:
        {
            std::uint16_t Addr = AddrAbsY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_INDX:
        {
            std::uint16_t Addr = AddrIndX(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CMP_INDY:
        {
            std::uint16_t Addr = AddrIndY(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(A, Operand);
        }break;

        case INS_CPX_IMM:
        {
            std::uint8_t Operand = FetchByte(Cycles, bus);
            SetStatusFlagsCompare(X, Operand);
        }break;

        case INS_CPX_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(X, Operand);
        }break;

        case INS_CPX_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(X, Operand);
        }break;

        case INS_CPY_IMM:
        {
            std::uint8_t Operand = FetchByte(Cycles, bus);
            SetStatusFlagsCompare(Y, Operand);
        }break;

        case INS_CPY_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(Y, Operand);
        }break;

        case INS_CPY_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus);
            std::uint8_t Operand = ReadByte(Cycles, Addr, bus);
            SetStatusFlagsCompare(Y, Operand);
        }break;

        case INS_ASL_ACC:
        {
            std::uint16_t  Temp = A << 1;
            SetStatusFlagsShiftLeft(Temp);
            A = (Temp & 0xFF);
            Cycles++;
        }break;

        case INS_ASL_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint16_t  Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
            SetStatusFlagsShiftLeft(Temp);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_ASL_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint16_t  Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
            SetStatusFlagsShiftLeft(Temp);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_ASL_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint16_t  Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
            SetStatusFlagsShiftLeft(Temp);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_ASL_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint16_t  Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
            SetStatusFlagsShiftLeft(Temp);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_LSR_ACC:
        {
            std::uint8_t Temp = A >> 1;
            SetStatusFlagsShiftRight(Temp, A);
            A = Temp;
            Cycles++;
        }break;

        case INS_LSR_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint8_t Temp = Value >> 1;
            SetStatusFlagsShiftRight(Temp, Value);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_LSR_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint8_t Temp = Value >> 1;
            SetStatusFlagsShiftRight(Temp, Value);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_LSR_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint8_t Temp = Value >> 1;
            SetStatusFlagsShiftRight(Temp, Value);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_LSR_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //getting the address
            std::uint8_t Value = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            std::uint8_t Temp = Value >> 1;
            SetStatusFlagsShiftRight(Temp, Value);
            WriteByte((Temp & 0xFF), Cycles, Addr, bus);
            Cycles++;
        }break;

        case INS_ROL_ACC:
        {
            RotateLeft(A, 0x0000, Cycles, bus, "A");
        }break;

        case INS_ROL_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateLeft(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROL_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateLeft(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROL_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateLeft(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROL_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateLeft(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROR_ACC:
        {
            RotateRight(A, 0x0000, Cycles, bus, "A");
        }break;

        case INS_ROR_ZP:
        {
            std::uint16_t Addr = AddrZeroPage(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateRight(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROR_ZPX:
        {
            std::uint16_t Addr = AddrZeroPageX(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateRight(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROR_ABS:
        {
            std::uint16_t Addr = AddrAbs(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateRight(M, Addr, Cycles, bus, "M");
        }break;

        case INS_ROR_ABSX:
        {
            std::uint16_t Addr = AddrAbsXWrite(Cycles, bus); //getting the address
            std::uint8_t M = ReadByte(Cycles, Addr, bus); //getting the value stored at this address
            RotateRight(M, Addr, Cycles, bus, "M");
        }break;

        default:
        {
            printf("Instruction not handled 0x%02x\n", Ins);
            throw -1;
        }
    }
}

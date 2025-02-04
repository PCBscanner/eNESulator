#pragma once
#include "../src/bus.h"
#include "../src/ppu.h"

using Word = std::uint16_t; //16-bit

struct CPU
{

    Word PC = 0xFFFC; //Program Counter
    std::uint8_t SP = 0xFD; //Stack Pointer. Address of the current part of the stack (also known as Stack Register).
    std::uint8_t PS = 0b00100100; //Processor Status

    //Registers
    std::uint8_t A = 0x00; //accumulator
    std::uint8_t X = 0x00; //Index Register X
    std::uint8_t Y = 0x00; //Index Register Y

    std::uint8_t DMAValue = 0x00;

    //BOOLS FOR TRACKING NMI
    bool NMIAllowed = true; //potentially set to false and set to true after a set number of cycles?
    bool NMIActive = false;

    //OPCODES
    //LDA
    static constexpr std::uint8_t INS_LDA_IM   = 0xA9; //load accumulator in immediate mode
    static constexpr std::uint8_t INS_LDA_ZP   = 0xA5; //load accumulator in zero page mode
    static constexpr std::uint8_t INS_LDA_ZPX  = 0xB5; //load accumulator in zero page,X mode
    static constexpr std::uint8_t INS_LDA_ABS  = 0xAD; //load accumulator in absolute mode
    static constexpr std::uint8_t INS_LDA_ABSX = 0xBD; //load accumulator in absolute,X mode
    static constexpr std::uint8_t INS_LDA_ABSY = 0xB9; //load accumulator in absolute,Y mode
    static constexpr std::uint8_t INS_LDA_INDX = 0xA1; //load accumulator in indirect,X mode
    static constexpr std::uint8_t INS_LDA_INDY = 0xB1; //load accumulator in indirect,Y mode
    //LDX
    static constexpr std::uint8_t INS_LDX_IM   = 0xA2; //load X register in immediate mode
    static constexpr std::uint8_t INS_LDX_ZP   = 0xA6; //load X register in zero page mode
    static constexpr std::uint8_t INS_LDX_ZPY  = 0xB6; //load X register in zero page,Y mode
    static constexpr std::uint8_t INS_LDX_ABS  = 0xAE; //load X register in absolute mode
    static constexpr std::uint8_t INS_LDX_ABSY = 0xBE; //load X register in absolute,Y mode
    //LDY
    static constexpr std::uint8_t INS_LDY_IM   = 0xA0; //load Y register in immediate mode
    static constexpr std::uint8_t INS_LDY_ZP   = 0xA4; //load Y register in zero page mode
    static constexpr std::uint8_t INS_LDY_ZPX  = 0xB4; //load Y register in zero page,X mode
    static constexpr std::uint8_t INS_LDY_ABS  = 0xAC; //load Y register in absolute mode
    static constexpr std::uint8_t INS_LDY_ABSX = 0xBC; //load Y register in absolute,X mode
    //STA
    static constexpr std::uint8_t INS_STA_ZP   = 0x85; //store accumulator in zero page mode
    static constexpr std::uint8_t INS_STA_ZPX  = 0x95; //store accumulator in zero page,X mode
    static constexpr std::uint8_t INS_STA_ABS  = 0x8D; //store accumulator in absolute mode
    static constexpr std::uint8_t INS_STA_ABSX = 0x9D; //store accumulator in absolute mode
    static constexpr std::uint8_t INS_STA_ABSY = 0x99; //store accumulator in absolute mode
    static constexpr std::uint8_t INS_STA_INDX = 0x81; //store accumulator in indirect,X mode
    static constexpr std::uint8_t INS_STA_INDY = 0x91; //store accumulator in indirect,Y mode
    //STX
    static constexpr std::uint8_t INS_STX_ZP   = 0x86; //store X register in zero page mode
    static constexpr std::uint8_t INS_STX_ZPY  = 0x96; //store X register in zero page,Y mode
    static constexpr std::uint8_t INS_STX_ABS  = 0x8E; //store register X in absolute mode
    //STY
    static constexpr std::uint8_t INS_STY_ZP   = 0x84; //store X register in zero page mode
    static constexpr std::uint8_t INS_STY_ZPX  = 0x94; //store Y register in zero page,X mode
    static constexpr std::uint8_t INS_STY_ABS  = 0x8C; //store register Y in absolute mode
    //JUMPS & CALLS
    static constexpr std::uint8_t INS_JMP_ABS  = 0x4C; //jump to the address specified by the operand, absolute mode
    static constexpr std::uint8_t INS_JMP_IND  = 0x6C; //jump to the address specified by the operand, indirect mode
    static constexpr std::uint8_t INS_JSR      = 0x20; //jump to subroutine (in absolute mode - no other modes available for 6502)
    static constexpr std::uint8_t INS_RTS      = 0x60; //return to subroutine;
    //STACK OPERATIONS
    static constexpr std::uint8_t INS_TSX      = 0xBA; //store the stack pointer into the X register
    static constexpr std::uint8_t INS_TXS      = 0x9A; //store the X register into the stack pointer
    static constexpr std::uint8_t INS_PHA      = 0x48; //push the accumulator onto the stack
    static constexpr std::uint8_t INS_PHP      = 0x08; //push the processor status onto the stack
    static constexpr std::uint8_t INS_PLA      = 0x68; //pull a byte from the stack into the A register
    static constexpr std::uint8_t INS_PLP      = 0x28; //pull a byte from the stack into the processor status
    //LOGICAL AND
    static constexpr std::uint8_t INS_AND_IM   = 0x29; //logical AND in immediate mode
    static constexpr std::uint8_t INS_AND_ZP   = 0x25; //logical AND in zero page mode
    static constexpr std::uint8_t INS_AND_ZPX  = 0x35; //logical AND in zero page mode
    static constexpr std::uint8_t INS_AND_ABS  = 0x2D; //logical AND in absolute page mode
    static constexpr std::uint8_t INS_AND_ABSX = 0x3D;
    static constexpr std::uint8_t INS_AND_ABSY = 0x39;
    static constexpr std::uint8_t INS_AND_INDX = 0x21;
    static constexpr std::uint8_t INS_AND_INDY = 0x31;
    static constexpr std::uint8_t INS_EOR_IM   = 0x49; //logical EOR in immediate mode
    static constexpr std::uint8_t INS_EOR_ZP   = 0x45; //logical EOR in zero page mode
    static constexpr std::uint8_t INS_EOR_ZPX  = 0x55; //logical EOR in zero page mode
    static constexpr std::uint8_t INS_EOR_ABS  = 0x4D; //logical EOR in absolute mode
    static constexpr std::uint8_t INS_EOR_ABSX = 0x5D;
    static constexpr std::uint8_t INS_EOR_ABSY = 0x59;
    static constexpr std::uint8_t INS_EOR_INDX = 0x41;
    static constexpr std::uint8_t INS_EOR_INDY = 0x51;
    static constexpr std::uint8_t INS_ORA_IM   = 0x09; //logical OR in immediate mode
    static constexpr std::uint8_t INS_ORA_ZP   = 0x05; //logical OR in zero page mode
    static constexpr std::uint8_t INS_ORA_ZPX  = 0x15; //logical OR in zero page mode
    static constexpr std::uint8_t INS_ORA_ABS  = 0x0D; //logical OR in absolute mode
    static constexpr std::uint8_t INS_ORA_ABSX = 0x1D;
    static constexpr std::uint8_t INS_ORA_ABSY = 0x19;
    static constexpr std::uint8_t INS_ORA_INDX = 0x01;
    static constexpr std::uint8_t INS_ORA_INDY = 0x11;
    static constexpr std::uint8_t INS_BIT_ZP   = 0x24;
    static constexpr std::uint8_t INS_BIT_ABS  = 0x2C;
    //REGISTER TRANSFERS
    static constexpr std::uint8_t INS_TAX      = 0xAA;
    static constexpr std::uint8_t INS_TAY      = 0xA8;
    static constexpr std::uint8_t INS_TXA      = 0x8A;
    static constexpr std::uint8_t INS_TYA      = 0x98;
    //INCREMENT AND DECREMENT
    static constexpr std::uint8_t INS_INX      = 0xE8;
    static constexpr std::uint8_t INS_INY      = 0xC8;
    static constexpr std::uint8_t INS_DEX      = 0xCA;
    static constexpr std::uint8_t INS_DEY      = 0x88;
    static constexpr std::uint8_t INS_INC_ZP   = 0xE6;
    static constexpr std::uint8_t INS_INC_ZPX  = 0xF6;
    static constexpr std::uint8_t INS_INC_ABS  = 0xEE;
    static constexpr std::uint8_t INS_INC_ABSX = 0xFE;
    static constexpr std::uint8_t INS_DEC_ZP   = 0xC6;
    static constexpr std::uint8_t INS_DEC_ZPX  = 0xD6;
    static constexpr std::uint8_t INS_DEC_ABS  = 0xCE;
    static constexpr std::uint8_t INS_DEC_ABSX = 0xDE;
    //BRANCHES
    static constexpr std::uint8_t INS_BCC      = 0x90;
    static constexpr std::uint8_t INS_BCS      = 0xB0;
    static constexpr std::uint8_t INS_BEQ      = 0xF0;
    static constexpr std::uint8_t INS_BMI      = 0x30;
    static constexpr std::uint8_t INS_BNE      = 0xD0;
    static constexpr std::uint8_t INS_BPL      = 0x10;
    static constexpr std::uint8_t INS_BVC      = 0x50;
    static constexpr std::uint8_t INS_BVS      = 0x70;

    static constexpr std::uint8_t INS_CLC      = 0x18;
    static constexpr std::uint8_t INS_CLD      = 0xD8;
    static constexpr std::uint8_t INS_CLI      = 0x58;
    static constexpr std::uint8_t INS_CLV      = 0xB8;
    static constexpr std::uint8_t INS_SEC      = 0x38;
    static constexpr std::uint8_t INS_SED      = 0xF8;
    static constexpr std::uint8_t INS_SEI      = 0x78;
    //SYSTEM FUNCTIONS
    static constexpr std::uint8_t INS_BRK      = 0x00;
    static constexpr std::uint8_t INS_RTI      = 0x40;
    static constexpr std::uint8_t INS_NOP      = 0xEA;
    //ARTITHMETIC
    static constexpr std::uint8_t INS_ADC_IMM  = 0x69;
    static constexpr std::uint8_t INS_ADC_ZP   = 0x65;
    static constexpr std::uint8_t INS_ADC_ZPX  = 0x75;
    static constexpr std::uint8_t INS_ADC_ABS  = 0x6D;
    static constexpr std::uint8_t INS_ADC_ABSX = 0x7D;
    static constexpr std::uint8_t INS_ADC_ABSY = 0x79;
    static constexpr std::uint8_t INS_ADC_INDX = 0x61;
    static constexpr std::uint8_t INS_ADC_INDY = 0x71;
    static constexpr std::uint8_t INS_SBC_IMM  = 0xE9;
    static constexpr std::uint8_t INS_SBC_ZP   = 0xE5;
    static constexpr std::uint8_t INS_SBC_ZPX  = 0xF5;
    static constexpr std::uint8_t INS_SBC_ABS  = 0xED;
    static constexpr std::uint8_t INS_SBC_ABSX = 0xFD;
    static constexpr std::uint8_t INS_SBC_ABSY = 0xF9;
    static constexpr std::uint8_t INS_SBC_INDX = 0xE1;
    static constexpr std::uint8_t INS_SBC_INDY = 0xF1;
    static constexpr std::uint8_t INS_CMP_IMM  = 0xC9;
    static constexpr std::uint8_t INS_CMP_ZP   = 0xC5;
    static constexpr std::uint8_t INS_CMP_ZPX  = 0xD5;
    static constexpr std::uint8_t INS_CMP_ABS  = 0xCD;
    static constexpr std::uint8_t INS_CMP_ABSX = 0xDD;
    static constexpr std::uint8_t INS_CMP_ABSY = 0xD9;
    static constexpr std::uint8_t INS_CMP_INDX = 0xC1;
    static constexpr std::uint8_t INS_CMP_INDY = 0xD1;
    static constexpr std::uint8_t INS_CPX_IMM  = 0xE0;
    static constexpr std::uint8_t INS_CPX_ZP   = 0xE4;
    static constexpr std::uint8_t INS_CPX_ABS  = 0xEC;
    static constexpr std::uint8_t INS_CPY_IMM  = 0xC0;
    static constexpr std::uint8_t INS_CPY_ZP   = 0xC4;
    static constexpr std::uint8_t INS_CPY_ABS  = 0xCC;
    //SHIFTS
    static constexpr std::uint8_t INS_ASL_ACC  = 0x0A;
    static constexpr std::uint8_t INS_ASL_ZP   = 0x06;
    static constexpr std::uint8_t INS_ASL_ZPX  = 0x16;
    static constexpr std::uint8_t INS_ASL_ABS  = 0x0E;
    static constexpr std::uint8_t INS_ASL_ABSX = 0x1E;
    static constexpr std::uint8_t INS_LSR_ACC  = 0x4A;
    static constexpr std::uint8_t INS_LSR_ZP   = 0x46;
    static constexpr std::uint8_t INS_LSR_ZPX  = 0x56;
    static constexpr std::uint8_t INS_LSR_ABS  = 0x4E;
    static constexpr std::uint8_t INS_LSR_ABSX = 0x5E;
    static constexpr std::uint8_t INS_ROL_ACC  = 0x2A;
    static constexpr std::uint8_t INS_ROL_ZP   = 0x26;
    static constexpr std::uint8_t INS_ROL_ZPX  = 0x36;
    static constexpr std::uint8_t INS_ROL_ABS  = 0x2E;
    static constexpr std::uint8_t INS_ROL_ABSX = 0x3E;
    static constexpr std::uint8_t INS_ROR_ACC  = 0x6A;
    static constexpr std::uint8_t INS_ROR_ZP   = 0x66;
    static constexpr std::uint8_t INS_ROR_ZPX  = 0x76;
    static constexpr std::uint8_t INS_ROR_ABS  = 0x6E;
    static constexpr std::uint8_t INS_ROR_ABSX = 0x7E;

    //RESETTING THE CPU
    std::uint32_t Reset(Bus& bus, PPU& ppu) //returning the number of cycles used on a reset
    {
        PC = 0xFFFC; //6502 reads the reset vector from 0xFFFC and 0xFFFD
        SP -= 3;
        PS |= 0b00100100; //TBC what this should actually be
        A = X = Y = 0x00;
        std::uint32_t Cycles = 0;
        // printf("PC: 0x%04x\n", PC);
        PC = FetchWord(Cycles, bus, ppu);
        // printf("PC: 0x%04x\n", PC);
        return Cycles += 5; //total number of cycles used for the reset is 7
    }

    //FETCHING, READING, AND WRITING, FROM MEMORY
    std::uint8_t FetchByte( std::uint32_t& Cycles, Bus& bus, PPU& ppu ) // reading from where the program counter is right now and then incrementing it
    {
        std::uint8_t Data = bus.ReadCPUBus(PC, ppu);
        // std::uint8_t Data = memory[PC];
        PC++; //incrementing the program counter
        Cycles++;
        return Data;
    }

    Word FetchWord( std::uint32_t& Cycles, Bus& bus, PPU& ppu ) // reading from where the program counter is right now and then incrementing it (twice)
    {
        //6502 is little endian
        //first byte is LSB
        std::uint8_t LoByte = FetchByte(Cycles, bus, ppu);
        // printf("PC: 0x%04x\n", PC);
        std::uint8_t HiByte = FetchByte(Cycles, bus, ppu);
        return LoByte | (HiByte << 8); //bit-shifting and using logical OR to get the 16-bit (2 byte) word.
    }

    std::uint8_t ReadByte( std::uint32_t& Cycles, Word Address, Bus& bus, PPU& ppu ) //reading an 8-bit from a given 16-bit memory address.
    //note that we do not increment the program counter for a read byte. Why?
    {
        std::uint8_t Data = bus.ReadCPUBus(Address, ppu);
        Cycles++;
        return Data;
    }

    Word ReadWord( std::uint32_t& Cycles, Word Address, Bus& bus, PPU& ppu ) //reading a 16-bit value from a given 16-bit memory address.
    //note that we do not increment the program counter for a read byte
    {
        std::uint8_t LoByte = ReadByte (Cycles, Address, bus, ppu);
        std::uint8_t HiByte = ReadByte (Cycles, Address+1, bus, ppu);
        return LoByte | (HiByte << 8);
    }

    //write 1 byte to memory
    void WriteByte(std::uint8_t Value, std::uint32_t& Cycles, Word Address, Bus& bus, PPU& ppu)
    {
        bus.WriteCPUBus(Value, Address, ppu);
        // memory[Address] = Value;
        Cycles++;
    }

    //write 2 bytes to memory
    void WriteWord( Word Value, std::uint32_t& Cycles, Word Address, Bus& bus, PPU& ppu )
    {
        bus.WriteCPUBus((Value & 0xFF), Address, ppu);
        Cycles++;
        bus.WriteCPUBus((Value >> 8), Address+1, ppu);
        Cycles --;
    }

    //STACK OPERATIONS
    //returning the 8-bit stack pointer as a full 16-bit address starting at 0x0100 (1st page).
    Word SPToAddr(std::uint8_t SP) const
    {
        return 0x0100 | SP;
    }

    //push the program counter to the stack
    void PushPCToStack(std::uint32_t& Cycles, Bus& bus, PPU& ppu, std::uint32_t Offset)
    {
        WriteByte((PC+Offset & 0xFF00)>>8, Cycles, SPToAddr(SP), bus, ppu); //writing the HiByte first
        SP--; //decrementing the SP after we push
        WriteByte((PC+Offset & 0x00FF), Cycles, SPToAddr(SP), bus, ppu); //writing the LoByte
        SP--; //decrementing the SP after we push
        Cycles++; //Is this extra cycle correct here?
    }

    //pushing a single byte to the stack
    void PushByteOntoStack(std::uint8_t Value, std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        WriteByte(Value, Cycles, SPToAddr(SP), bus, ppu);
        SP--; //decrementing the SP after we push
        Cycles++;
    }

    std::uint8_t PullByteFromStack(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        SP++; //incrementing the SP before we pull
        Cycles++;
        std::uint8_t Value = ReadByte(Cycles, SPToAddr(SP), bus, ppu);
        return Value;
    }

    Word PullWordFromStack(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        SP++; //incrementing the SP before we pull
        Cycles++;
        std::uint8_t LoByte = ReadByte(Cycles, SPToAddr(SP), bus, ppu);
        SP++; //incrementing the SP before we pull
        Cycles++;
        std::uint8_t HiByte = ReadByte(Cycles, SPToAddr(SP), bus, ppu);
        return LoByte | (HiByte << 8);;
    }

    //SETTING PROCESSOR STATUS FLAGS
    //sets the correct processor status after loading a value
    void LDValueSetProcessorStatus(std::uint8_t Value)
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

    void SetStatusFlagsADC(std::uint8_t A, std::uint8_t AInit, std::uint8_t Operand, Word AWord)
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

    void SetStatusFlagsSBC(std::uint8_t A, std::uint8_t AInit, std::uint8_t Operand, Word AWord)
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

    void SetStatusFlagsCompare(std::uint8_t Register, std::uint8_t Operand)
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

    void SetStatusFlagsShiftLeft(Word ValueShifted)
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

    void SetStatusFlagsShiftRight(std::uint8_t ValueShifted, std::uint8_t Value)
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

    void SetStatusFlagsRotateLeft(Word ValueShifted)
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

    void SetStatusFlagsRotateRight(std::uint8_t ValueShifted, std::uint8_t Value)
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
    void CalculateADC(std::uint8_t Operand)
    {
        std::uint32_t CarryFlag = (PS & 0b00000001);
        if ((PS & 0b00001000) != 0)
        {
            printf("Decimal mode not yet implemented.\n");
            // throw -1;
        }
        Word AWord = A; //initially converting to 16-bit
        std::uint8_t AInit = A; //used for determining the overflow flag
        AWord += Operand;
        AWord += CarryFlag;
        A = ( AWord & 0xFF ); //converting back to an 8-bit number
        SetStatusFlagsADC(A, AInit, Operand, AWord);
    }

    void CalculateSBC(std::uint8_t Operand)
    {
        std::uint32_t CarryFlag = (PS & 0b00000001);
        if ((PS & 0b00001000) != 0)
        {
            printf("Decimal mode not yet implemented.\n");
            // throw -1;
        }
        Word AWord = A; //initially converting to 16-bit
        std::uint8_t AInit = A; //used for determining the overflow flag
        AWord += ((~Operand));
        AWord += CarryFlag;
        A = ( AWord & 0xFF ); //converting back to an 8-bit number
        SetStatusFlagsSBC(A, AInit, Operand, AWord);
    }

    //SHIFT OPERATIONS
    void RotateLeft(std::uint8_t Value, Word Addr, std::uint32_t& Cycles, Bus& bus, PPU& ppu, std::string AddrMode)
    {
        Word Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
        Temp += (PS & 0b00000001); //setting bit 0 to the original carry bit
        SetStatusFlagsRotateLeft(Temp);
        if (AddrMode == "A") //Accumulator addressing mode
        {
            A = (Temp & 0xFF);  //extracting the LSB from the word
        }
        else
        {
            WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
        }
        Cycles++;
    }

    void RotateRight(std::uint8_t Value, Word Addr, std::uint32_t& Cycles, Bus& bus, PPU& ppu, std::string AddrMode)
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
            WriteByte(Temp, Cycles, Addr, bus, ppu);
        }
        Cycles++;
    }

    //ADDRESSING MODES
    Word AddrZeroPage( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus, ppu);
        return ZeroPageAddr;
    }

    Word AddrZeroPageX( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus, ppu);
        ZeroPageAddr += X; //wrap account for in the variable type
        Cycles++;
        return ZeroPageAddr;
    }

    Word AddrZeroPageY( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte( Cycles, bus, ppu);
        ZeroPageAddr += Y; //wrap account for in the variable type
        Cycles++;
        return ZeroPageAddr;
    }

    Word AddrAbs( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        Word AbsAddr = FetchWord(Cycles, bus, ppu);
        return AbsAddr;
    }

    Word AddrAbsX( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        Word AbsAddr = FetchWord(Cycles, bus, ppu); //2 cycles to fetch the word
        Word AbsAddrX = AbsAddr + X; //WHY DOES THIS NOT COST A CYCLE?
        //checking if the page boundary has been crossed
        //the operand's low byte is fetched before the high byte, so the processor can start adding the X register's value before it has the high byte. If there is no carry operation, the entire indexed operation takes only four clocks,
        if ( ( (AbsAddr & 0xFF) + X) > 0xFF)
        {
            Cycles++;
        }
        return AbsAddrX;
    }

    Word AddrAbsXWrite( std::uint32_t& Cycles, Bus& bus, PPU& ppu) //used for instructions that always use an additional cycle regardless of if a page boundary has been crossed
    {
        Word AbsAddr = FetchWord(Cycles, bus, ppu); //2 cycles to fetch the word
        Word AbsAddrX = AbsAddr + X;
        Cycles++;
        return AbsAddrX;
    }

    Word AddrAbsY( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        Word AbsAddr = FetchWord(Cycles, bus, ppu); //2 cycles to fetch the word
        Word AbsAddrY = AbsAddr + Y; //WHY DOES THIS NOT COST A CYCLE?
        //checking if the page boundary has been crossed
        //the operand's low byte is fetched before the high byte, so the processor can start adding the X register's value before it has the high byte. If there is no carry operation, the entire indexed operation takes only four clocks,
        if ( ( (AbsAddr & 0xFF) + Y) > 0xFF)
        {
            Cycles++;
        }
        return AbsAddrY;
    }

    Word AddrAbsYSTA( std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        Word AbsAddr = FetchWord(Cycles, bus, ppu); //2 cycles to fetch the word
        Word AbsAddrY = AbsAddr + Y;
        Cycles++;
        return AbsAddrY;
    }

    Word AddrIndX(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus, ppu);
        std::uint8_t AddrTmp = ((ZeroPageAddr + X)&0xFF);
        Cycles++;
        std::uint8_t LoByte = ReadByte(Cycles, AddrTmp, bus, ppu);
        AddrTmp++;
        std::uint8_t HiByte = ReadByte(Cycles, (AddrTmp&0xFF), bus, ppu);
        Word AddrIndX = LoByte | (HiByte << 8);
        return AddrIndX;
    }

    Word AddrIndY(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus, ppu);
        std::uint8_t LoByte = ReadByte(Cycles, ZeroPageAddr, bus, ppu);
        std::uint8_t HiByte = ReadByte(Cycles, ((ZeroPageAddr+1)&0xFF), bus, ppu);
        Word AddrTmp = LoByte | (HiByte << 8);
        Word AddrIndY = AddrTmp + Y;
        if ( ( ( AddrTmp & 0xFF ) + Y ) > 0xFF )
        {
            Cycles++;
        }
        return AddrIndY;
    }

    //adding a separate IndY addressing mode for STA because we always consume 6 cycles regardless of if we cross a page boundary
    Word AddrIndYSTA(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        std::uint8_t ZeroPageAddr = FetchByte(Cycles, bus, ppu);
        std::uint8_t LoByte = ReadByte(Cycles, ZeroPageAddr, bus, ppu);
        std::uint8_t HiByte = ReadByte(Cycles, ((ZeroPageAddr+1)&0xFF), bus, ppu);
        Word AddrTmp = LoByte | (HiByte << 8);
        Word AddrIndY = AddrTmp + Y;
        Cycles++;
        return AddrIndY;
    }

    //LOADING REGISTERS
    //loading a register with the value from the location specified in the PC
    void LoadRegFromPC(std::uint8_t& Register, std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        Register = FetchByte(Cycles, bus, ppu);
        LDValueSetProcessorStatus(Register);
    }

    //loading a register with the value from the location specified by an address
    void LoadRegFromAddr(std::uint8_t& Register, std::uint32_t& Cycles, Word Address, Bus& bus, PPU& ppu)
    {
        Register = ReadByte(Cycles, Address, bus, ppu);
        LDValueSetProcessorStatus(Register);
    }

    //BRANCHING
    void BranchToLocn(std::int8_t Displacement, std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        PC = PC + Displacement;
        Cycles++;
        if ( ( ((PC-Displacement) & 0x00FF) + Displacement) > 0x00FF ) //checking if we crossed into a new page
        {
            Cycles++;
        }
    }

    //ACTIVATING THE NMI INSTRUCTION
    void ExecuteNMI(std::uint32_t& Cycles, Bus& bus, PPU& ppu)
    {
        PushPCToStack(Cycles, bus, ppu, 0);
        PushByteOntoStack(PS, Cycles, bus, ppu);
        PC = ReadWord(Cycles, 0xFFFA, bus, ppu);
    }

    //EXECUTING INSTRUCTIONS
    void Execute( std::uint32_t& Cycles, Bus& bus, PPU& ppu )
    {
        // printf("PC: 0x%04x\n", PC);
        std::uint8_t Ins = FetchByte(Cycles, bus, ppu);
        // printf("Ins: 0x%02x\n", Ins);
        switch ( Ins )
        {
            case INS_LDA_IM:
            {
                LoadRegFromPC(A, Cycles, bus, ppu);
            } break;

            case INS_LDX_IM:
            {
                LoadRegFromPC(X, Cycles, bus, ppu);
            } break;

            case INS_LDY_IM:
            {
                LoadRegFromPC(Y, Cycles, bus, ppu);
            } break;

            case INS_LDA_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDX_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                LoadRegFromAddr(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDY_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                LoadRegFromAddr(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDA_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);;
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDY_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);;
                LoadRegFromAddr(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDA_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                // printf("Addr: 0x%04x\n", Addr);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDX_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                LoadRegFromAddr(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDY_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                LoadRegFromAddr(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDA_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDY_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                LoadRegFromAddr(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDA_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDX_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                LoadRegFromAddr(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDX_ZPY:
            {
                Word Addr = AddrZeroPageY(Cycles, bus, ppu);;
                LoadRegFromAddr(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_LDA_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_LDA_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                LoadRegFromAddr(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STA_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_STX_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                WriteByte(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_STY_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                WriteByte(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_STA_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            } break;

            case INS_STX_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                WriteByte(X, Cycles, Addr, bus, ppu);
            } break;

            case INS_STY_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                WriteByte(Y, Cycles, Addr, bus, ppu);
            } break;

            case INS_STA_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STY_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                WriteByte(Y, Cycles, Addr, bus, ppu);
            }break;

            case INS_STA_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STA_ABSY:
            {
                Word Addr = AddrAbsYSTA(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STA_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STA_INDY:
            {
                Word Addr = AddrIndYSTA(Cycles, bus, ppu);
                WriteByte(A, Cycles, Addr, bus, ppu);
            }break;

            case INS_STX_ZPY:
            {
                Word Addr = AddrZeroPageY(Cycles, bus, ppu);
                WriteByte(X, Cycles, Addr, bus, ppu);
            }break;


            case INS_JSR:
            {
                Word PCInit = PC; //taking a copy of the program counter
                std::uint8_t LoByte = ReadByte(Cycles, PCInit, bus, ppu); //fetching the low byte of the address
                PushPCToStack(Cycles, bus, ppu, 1);
                std::uint8_t HiByte = ReadByte(Cycles, PCInit+1, bus, ppu); //fetching the high byte of the address AFTER pushing PC to stack.
                PC = LoByte | (HiByte << 8);

                // Word SubAddr = FetchWord(Cycles, bus, ppu); //fetching the current value of the PC
                // PushPCToStack(Cycles, memory, -1);
                // PC = SubAddr;
            }break;

            case INS_RTS:
            {
                Word ReturnAddr = PullWordFromStack(Cycles, bus, ppu);
                PC = ReturnAddr + 1; //the return address points at the last byte of the previous instruction, therefore we add 1.
                Cycles++;
            }break;

            case INS_JMP_ABS:
            {
                Word Addr = FetchWord(Cycles, bus, ppu);
                PC = Addr;
            } break;

            case INS_JMP_IND:
            {
                //workaround for incorrect operation of the original 6502
                //An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page.
                Word AddrIndirect = FetchWord(Cycles, bus, ppu);
                std::uint8_t LoByte = ReadByte(Cycles, AddrIndirect, bus, ppu);
                std::uint8_t HiByte = 0;
                if ((AddrIndirect & 0xFF) == 0xFF)
                {
                    HiByte = ReadByte(Cycles, (AddrIndirect-0xFF), bus, ppu);
                }
                else
                {
                    HiByte = ReadByte(Cycles, (AddrIndirect+1), bus, ppu);
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
                PushByteOntoStack(A, Cycles, bus, ppu);
            }break;

            case INS_PHP:
            {
                //bits 4 and 5 are always set when we push to the stack.
                PushByteOntoStack((PS | 0b00110000), Cycles, bus, ppu);
            }break;

            case INS_PLA:
            {
                A = PullByteFromStack(Cycles, bus, ppu);
                Cycles++;
                LDValueSetProcessorStatus(A);
            }break;

            case INS_PLP:
            {
                //bits 4 and 5 are unaffected by PLP. We only want to update bits 0-3 and 6-7 from the stack.
                PS = (PS & 0b00110000) | (PullByteFromStack(Cycles, bus, ppu) & 0b11001111);
                Cycles++;
            }break;

            case INS_AND_IM:
            {
                A &= FetchByte(Cycles, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_IM:
            {
                A ^= FetchByte(Cycles, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_IM:
            {
                A |= FetchByte(Cycles, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_AND_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                A &= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_EOR_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                A ^= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_ORA_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                A |= ReadByte(Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(A);
            }break;

            case INS_BIT_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //reading the value from memory
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
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //reading the value from memory
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
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value++;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_INC_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value++;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_INC_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value++;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_INC_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value++;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_DEC_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value--;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_DEC_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value--;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_DEC_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value--;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_DEC_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //1 cycle
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu);
                Value--;
                Cycles++;
                WriteByte(Value, Cycles, Addr, bus, ppu);
                LDValueSetProcessorStatus(bus.ReadCPUBus(Addr, ppu));
            }break;

            case INS_BEQ:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b00000010) != 0) //if the zero flag is set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BNE:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b00000010) == 0) //if the zero flag is not set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BCS:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b00000001) != 0) //if the zero flag is set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BCC:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b00000001) == 0) //if the zero flag is not set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;


            case INS_BMI:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b10000000) != 0) //if the flag is set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BPL:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b10000000) == 0) //if the flag is not set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BVS:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b01000000) != 0) //if the flag is set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
                }
            }break;

            case INS_BVC:
            {
                std::int8_t Displacement = static_cast<std::int8_t>(FetchByte(Cycles, bus, ppu));
                if ((PS & 0b01000000) == 0) //if the flag is not set, then we branch
                {
                    BranchToLocn(Displacement, Cycles, bus, ppu);
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
                PushPCToStack(Cycles, bus, ppu, +1); //3 cycles
                PushByteOntoStack((PS | 0b00010000), Cycles, bus, ppu); //set bit 4 on the PS we push to the stack (do not update actual PS)
                PC = ReadWord(Cycles, 0xFFFE, bus, ppu); //2 cycles
                PS |= 0b00000100; //setting bit 2 (I flag)
                Cycles--; //unsure why this is required?
            }break;

            case INS_RTI:
            {
                std::uint8_t PSFromStack = PullByteFromStack(Cycles, bus, ppu);
                PSFromStack &= 0b11101111; //clearing bit 4 from the PS we pulled from the stack
                PS &= 0b00100000; //retaining bit 5 from the original PS because it is unaffected by this operation
                PS |= PSFromStack;
                PC = PullWordFromStack(Cycles, bus, ppu);
                Cycles--; //unsure why this is required?
            }break;

            case INS_NOP:
            {
                Cycles++;
            }break;

            case INS_ADC_IMM:
            {
                std::uint8_t Operand = FetchByte(Cycles, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;

            case INS_ADC_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateADC(Operand);
            }break;
///////////////

            case INS_SBC_IMM:
            {
                std::uint8_t Operand = FetchByte(Cycles, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            case INS_SBC_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                CalculateSBC(Operand);
            }break;

            //////////////////////////

            case INS_CMP_IMM:
            {
                std::uint8_t Operand = FetchByte(Cycles, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_ABSX:
            {
                Word Addr = AddrAbsX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_ABSY:
            {
                Word Addr = AddrAbsY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_INDX:
            {
                Word Addr = AddrIndX(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CMP_INDY:
            {
                Word Addr = AddrIndY(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(A, Operand);
            }break;

            case INS_CPX_IMM:
            {
                std::uint8_t Operand = FetchByte(Cycles, bus, ppu);
                SetStatusFlagsCompare(X, Operand);
            }break;

            case INS_CPX_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(X, Operand);
            }break;

            case INS_CPX_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(X, Operand);
            }break;

            case INS_CPY_IMM:
            {
                std::uint8_t Operand = FetchByte(Cycles, bus, ppu);
                SetStatusFlagsCompare(Y, Operand);
            }break;

            case INS_CPY_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(Y, Operand);
            }break;

            case INS_CPY_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu);
                std::uint8_t Operand = ReadByte(Cycles, Addr, bus, ppu);
                SetStatusFlagsCompare(Y, Operand);
            }break;

            case INS_ASL_ACC:
            {
                Word Temp = A << 1;
                SetStatusFlagsShiftLeft(Temp);
                A = (Temp & 0xFF);
                Cycles++;
            }break;

            case INS_ASL_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                Word Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
                SetStatusFlagsShiftLeft(Temp);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_ASL_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                Word Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
                SetStatusFlagsShiftLeft(Temp);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_ASL_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                Word Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
                SetStatusFlagsShiftLeft(Temp);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_ASL_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                Word Temp = Value << 1; //shifting the value left by one.Note promotion to 16-bit to track what happens to bit 7
                SetStatusFlagsShiftLeft(Temp);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
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
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                std::uint8_t Temp = Value >> 1;
                SetStatusFlagsShiftRight(Temp, Value);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_LSR_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                std::uint8_t Temp = Value >> 1;
                SetStatusFlagsShiftRight(Temp, Value);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_LSR_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                std::uint8_t Temp = Value >> 1;
                SetStatusFlagsShiftRight(Temp, Value);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_LSR_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //getting the address
                std::uint8_t Value = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                std::uint8_t Temp = Value >> 1;
                SetStatusFlagsShiftRight(Temp, Value);
                WriteByte((Temp & 0xFF), Cycles, Addr, bus, ppu);
                Cycles++;
            }break;

            case INS_ROL_ACC:
            {
                RotateLeft(A, 0x0000, Cycles, bus, ppu, "A");
            }break;

            case INS_ROL_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateLeft(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROL_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateLeft(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROL_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateLeft(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROL_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateLeft(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROR_ACC:
            {
                RotateRight(A, 0x0000, Cycles, bus, ppu, "A");
            }break;

            case INS_ROR_ZP:
            {
                Word Addr = AddrZeroPage(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateRight(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROR_ZPX:
            {
                Word Addr = AddrZeroPageX(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateRight(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROR_ABS:
            {
                Word Addr = AddrAbs(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateRight(M, Addr, Cycles, bus, ppu, "M");
            }break;

            case INS_ROR_ABSX:
            {
                Word Addr = AddrAbsXWrite(Cycles, bus, ppu); //getting the address
                std::uint8_t M = ReadByte(Cycles, Addr, bus, ppu); //getting the value stored at this address
                RotateRight(M, Addr, Cycles, bus, ppu, "M");
            }break;

            default:
            {
                printf("Instruction not handled 0x%02x\n", Ins);
                throw -1;
            }
            }
    }

};

#pragma once
#include "../src/bus.h"
#include "../src/ppu.h"

struct Bus;

struct CPU
{
    Bus* bus_ptr;

    std::uint16_t PC;     //Program Counter
    std::uint8_t  SP;       //Stack Pointer. Address of the current part of the stack (also known as Stack Register).
    std::uint8_t  PS; //Processor Status

    //Registers
    std::uint8_t A; //accumulator
    std::uint8_t X; //Index Register X
    std::uint8_t Y; //Index Register Y

    std::uint8_t DMAValue;

    bool PutCycle;

    //BOOLS FOR TRACKING NMI
    bool NMIAllowed; //potentially set to false and set to true after a set number of cycles?
    bool NMIActive;

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

    void ConnectBus(Bus&);

    std::uint32_t Reset();

    std::uint8_t FetchByte(std::uint32_t&);

    std::uint16_t FetchWord( std::uint32_t&);

    std::uint8_t ReadByte( std::uint32_t&, std::uint16_t, Bus&);

    std::uint16_t ReadWord( std::uint32_t&, std::uint16_t, Bus&);

    void WriteByte(std::uint8_t, std::uint32_t&, std::uint16_t, Bus&);

    void WriteWord(std::uint16_t, std::uint32_t&, std::uint16_t, Bus&);

    std::uint16_t SPToAddr(std::uint8_t SP) const;

    void PushPCToStack(std::uint32_t&, Bus&, std::uint32_t);

    void PushByteOntoStack(std::uint8_t, std::uint32_t&, Bus&);

    std::uint8_t PullByteFromStack(std::uint32_t&, Bus&);

    std::uint16_t PullWordFromStack(std::uint32_t&, Bus&);

    void LDValueSetProcessorStatus(std::uint8_t);

    void SetStatusFlagsADC(std::uint8_t, std::uint8_t, std::uint8_t, std::uint16_t);

    void SetStatusFlagsSBC(std::uint8_t, std::uint8_t, std::uint8_t, std::uint16_t);

    void SetStatusFlagsCompare(std::uint8_t, std::uint8_t);

    void SetStatusFlagsShiftLeft(std::uint16_t);

    void SetStatusFlagsShiftRight(std::uint8_t, std::uint8_t);

    void SetStatusFlagsRotateLeft(std::uint16_t);

    void SetStatusFlagsRotateRight(std::uint8_t, std::uint8_t);

    void CalculateADC(std::uint8_t);

    void CalculateSBC(std::uint8_t);

    void RotateLeft(std::uint8_t, std::uint16_t, std::uint32_t&, Bus&, std::string);

    void RotateRight(std::uint8_t, std::uint16_t, std::uint32_t&, Bus&, std::string);

    std::uint16_t AddrZeroPage( std::uint32_t&, Bus&);

    std::uint16_t AddrZeroPageX( std::uint32_t&, Bus&);

    std::uint16_t AddrZeroPageY( std::uint32_t&, Bus&);

    std::uint16_t AddrAbs( std::uint32_t&, Bus&);

    std::uint16_t AddrAbsX( std::uint32_t&, Bus&);

    std::uint16_t AddrAbsXWrite( std::uint32_t&, Bus&);

    std::uint16_t AddrAbsY( std::uint32_t&, Bus&);

    std::uint16_t AddrAbsYSTA( std::uint32_t&, Bus&);

    std::uint16_t AddrIndX(std::uint32_t&, Bus&);

    std::uint16_t AddrIndY(std::uint32_t&, Bus&);

    std::uint16_t AddrIndYSTA(std::uint32_t&, Bus&);

    void LoadRegFromPC(std::uint8_t&, std::uint32_t&, Bus&);

    void LoadRegFromAddr(std::uint8_t&, std::uint32_t&, std::uint16_t, Bus&);

    void BranchToLocn(std::int8_t, std::uint32_t&, Bus&);

    void ExecuteNMI(std::uint32_t&, Bus&);

    void Execute(std::uint32_t&, Bus&);
};

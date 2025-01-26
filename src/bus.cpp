#include <SDL2/SDL.h>
#include "../src/bus.h"

void Bus::LoadCartridge(std::string Cartridge)
{
    std::ifstream stream(Cartridge, std::ios::in | std::ios::binary);
    std::vector<std::uint8_t> ROM;
    std::for_each(std::istreambuf_iterator<char>(stream),
                  std::istreambuf_iterator<char>(),
                  [&ROM](const char c){
                      ROM.push_back(c);
                  });

    std::uint32_t Signature = ROM[0]<<24 | ROM[1]<<16 | ROM[2]<<8 | ROM[3];
    if (Signature != 0x4e45531a)
    {
        printf("Not a valid iNES format. Terminating load.");
        throw -1;
    }

    std::uint8_t PRGROMSize  = ROM[4];  //size in multiples of 16 KB.
    std::uint8_t CHRROMSize  = ROM[5];  //size in multiples of 8 KB.
    std::uint8_t Flags6      = ROM[6];  //mapper, mirroring, battery, trainer
    std::uint8_t Flags7      = ROM[7];
    std::uint8_t Flags8      = ROM[8];
    std::uint8_t Flags9      = ROM[9];
    std::uint8_t Flags10     = ROM[10];

    std::uint8_t  Trainer = ((Flags6 & 0b00000100) >> 2);
    PRGROMStartAddr = 16 + (Trainer * 512);
    CHRROMStartAddr = PRGROMStartAddr + (PRGROMSize*16384);
    NametableMirror = Flags6 & 1;
    Mapper          = (Flags6 & 0b11110000) >> 4;

    // printf("Mapper: %03d\n", Mapper);
    //Loading PRG ROM to the CPU memory
    //This assumes that trainers are not included.

    //loading all of the PRGROM from ROM into the PRGROM array
    //this allows us to switch banks with the mapper
    for(std::uint32_t i=0; i<PRGROMSize*16384; i++)
    {
        PRGROM[i] = ROM[PRGROMStartAddr + i];
    }
    for(std::uint32_t i=0; i<CHRROMSize*8192; i++)
    {
        CHRROM[i] = ROM[CHRROMStartAddr + i];
    }

    if(Mapper == 0)
    {
        //mapping PRGROM
        if(PRGROMSize == 1)
        {
            for(std::uint32_t i=0; i<PRGROMSize*16384; i++)
            {
                CPUMemory[0x8000 + i] = ROM[PRGROMStartAddr + i];
                CPUMemory[0xC000 + i] = ROM[PRGROMStartAddr + i]; //mirroring
            }
        }
        else if(PRGROMSize == 2)
        {
            for(std::uint32_t i=0; i<PRGROMSize*16384; i++)
            {
                CPUMemory[0x8000 + i] = ROM[PRGROMStartAddr + i];
            }
        }
        //mapping CHRROM
        for(std::uint32_t i=0; i<CHRROMSize*8192; i++)
        {
            PPUMemory[i] = ROM[CHRROMStartAddr + i];
        }
    }
    else if(Mapper == 2)
    {
        //mapping first switchable bank
        //which bank should this default to on reset?
        for(std::uint32_t i=0; i<16384; i++)
        {
            CPUMemory[0x8000 + i] = PRGROM[i];
        }
        //mapping the fixed bank
        for(std::uint32_t i=0; i<16384; i++)
        {
            CPUMemory[0xC000 + i] = PRGROM[  ( (PRGROMSize - 1) * 16384 ) + i];
        }
        //mapping the CHRROM
        for(std::uint32_t i=0; i<CHRROMSize*8192; i++)
        {
            PPUMemory[i] = ROM[CHRROMStartAddr + i];
        }
    }


}

void Bus::SelectBank(std::uint8_t Bank)
{
    //000
    if(Mapper == 0)
    {
        //do nothing. we shouldn't ever reach here...
    }
    //002
    else if( (Mapper == 2) || (Mapper == 94) || (Mapper == 180))
    {
        for(std::uint32_t i=0; i<16384; i++)
        {
            CPUMemory[0x8000 + i] = PRGROM[(Bank * 16384) + i];
        }
    }
    else
    {
        printf("Bank switching for mapper %03d is not implemented.\n", Mapper);
        throw -1;
    }
}

void Bus::WriteCPUBus(std::uint8_t Value, std::uint16_t Addr, PPU& ppu)
{
    if(AddressMirroring == true)
    {
        if(Addr >= 0x0000 && Addr <= 0x1FFF) //mirrors of CPU RAM
        {
            Addr &= 0x07FF;
            CPUMemory[Addr] = Value;
        }
        else if(Addr >= 0x2000 && Addr <= 0x3FFF) //mirrors of PPU registers
        {
            Addr &= 0x2007;
            CPUMemory[Addr] = Value;
            WritePPUBus(Value, Addr, ppu);
        }
        else if(Addr == 0x4014) //OAMDMA
        {
            CPUMemory[Addr] = Value;
            WritePPUBus(Value, Addr, ppu);
        }
        else if(Addr == 0x4016)
        {
            if((Value&1) == 1) //https://forums.nesdev.org/viewtopic.php?t=13985
            {
                PollController();
            }
        }
        else if( (0x8000 <= Addr) && (Addr <= 0xFFFF)  ) //mapper select
        {
            SelectBank(Value);
        }
        else
        {
            CPUMemory[Addr] = Value;
        }
    }
    else
    {
        CPUMemory[Addr] = Value;
    }
}

std::uint8_t Bus::ReadCPUBus(std::uint16_t Addr, PPU& ppu)
{
    std::uint8_t Value = 0;
    if(AddressMirroring == true)
    {
        if(Addr >= 0x0000 && Addr <= 0x1FFF) //mirrors of CPU RAM
        {
            Addr &= 0x07FF;
            Value = CPUMemory[Addr];
        }
        else if(Addr >= 0x2000 && Addr <= 0x3FFF) //mirrors of PPU bus
        {
            Addr &= 0x2007;
            Value = CPUMemory[Addr];
            std::uint8_t ValuePPU = ReadPPUBus(Addr, ppu); //to make sure we set the right flags
        }
        else if(Addr == 0x4016)
        {
            Value = (ControllerP1Reg & 1) | 0b01000000; //extracting LSb.
            ControllerP1Reg = ControllerP1Reg >> 1;
        }
        else
        {
            Value = CPUMemory[Addr];
        }
    }
    else
    {
        Value = CPUMemory[Addr];
    }
    return Value;
}

void Bus::WritePPUBus(std::uint8_t Value, std::uint16_t Addr, PPU& ppu)
{
    switch (Addr)
    {
        case(0x2000): //PPUCTRL
        {
            ppu.PPUCTRL = Value;
            ppu.T = (ppu.T & 0b111001111111111) | ( (Value & 0b11) << 10 ); //base nametable address
            CPUMemory[Addr] = Value;
        }break;
        case(0x2001): //PPUMASK
        {
            if ( (Value & 0b00001000) != 0 )
            {
                ppu.RenderBackground = true;
            }
            else
            {
                ppu.RenderBackground = false;
            }
            if ( (Value & 0b00010000) != 0 )
            {
                ppu.RenderSprites = true;
            }
            else
            {
                ppu.RenderSprites = false;
            }
            ppu.PPUMASK = Value;
            CPUMemory[Addr] = Value;
        }break;
        case(0x2002): //PPUSTATUS
        {
            //READ ONLY
        }break;
        case(0x2003): //OAMADDR
        {
            ppu.OAMADDR = Value;
        }break;
        case(0x2004): //OAMDATA
        {

            OAM[ppu.OAMADDR] = Value;
            ppu.OAMADDR++;
        }break;
        case(0x2005): //PPUSCROLL
        {
            if(ppu.W == 0) //first write
            {
                ppu.T  = (ppu.T & 0b111111111100000) | (Value >> 3);    //coarse-x
                ppu.X  = (Value & 0b111); //fine-x
                ppu.W  = 1;               //address latch
            }
            else //second write
            {
                ppu.T  = (ppu.T & 0b000111111111111) | ( (Value & 0b00000111) << 12 ); //fine-y
                ppu.T  = (ppu.T & 0b111110000011111) | ( (Value & 0b11111000) << 2  ); //coarse-y
                ppu.W  = 0;
            }
        }break;
        case(0x2006): //PPUADDR
        {
            if(ppu.W == 0) //writing hi byte first
            {
                ppu.T  = ( ( Value & 0x3F ) << 8 ) | ( ppu.T & 0x00FF );
                ppu.T &= 0b011111111111111; //clearing bit 14 of t
                ppu.W  = 1;
            }
            else //then writing the lo byte if the latch has been reset
            {
                ppu.T = (ppu.T & 0xFF00) | Value;
                ppu.V =  ppu.T;
                ppu.W =  0;
            }
        }break;
        case(0x2007): //PPUDATA
        {
            std::uint16_t Addr = ppu.V;

            MirrorPPUAddr(Addr);

            PPUMemory[Addr] = Value;

            IncrementPPUADDR(ppu);
        }break;
        case(0x4014): //OAMDMA
        {
            ppu.OAMDMA = Value;
        }break;

    }
}

std::uint8_t Bus::ReadPPUBus(std::uint16_t Addr, PPU& ppu)
{
    std::uint8_t Value = 0;
    switch (Addr)
    {
        case(0x2000): //PPUCTRL
        {
            //WRITE ONLY
        }break;
        case(0x2001): //PPUMASK
        {
            //WRITE ONLY
        }break;
        case(0x2002): //PPUSTATUS
        {
            ppu.W = 0; //clearing the w-register on read.
            ppu.PPUSTATUS &= 0b01111111; //clearing vblank
            Value = (ppu.PPUSTATUS & 0xE0) | (ppu.PPUDATABuffer & 0x1F);
        }break;
        case(0x2003): //OAMADDR
        {
            //WRITE ONLY
        }break;
        case(0x2004): //OAMDATA
        {
             Value = OAM[ppu.OAMADDR];
        }break;
        case(0x2005): //PPUSCROLL
        {
             //WRITE ONLY
        }break;
        case(0x2006): //PPUADDR
        {
             //WRITE ONLY
        }break;
        case(0x2007): //PPUDATA
        {

            std::uint16_t Addr = ppu.V;

            MirrorPPUAddr(Addr);

            ppu.PPUDATA = ppu.PPUDATABuffer;

            //we shall return the PPU data buffer
            Value = ppu.PPUDATABuffer;

            //updating PPUDATABuffer
            ppu.PPUDATABuffer = PPUMemory[Addr];

            //...but we return the value immediately if we're reading from the palette
            if(Addr >= 0x3F00)
            {
                Value = ppu.PPUDATABuffer;
            }

            IncrementPPUADDR(ppu);
        }break;
        case(0x4014): //OAMDMA
        {
            //WRITE ONLY
        }break;
    }
    return Value;
}

void Bus::MirrorPPUAddr(std::uint16_t& Addr)
{
    Addr &= 0x3FFF;

    if( (0x2000 <= Addr) && (Addr <= 0x3EFF) )
    {
        Addr &= 0x2FFF;

        if(NametableMirror == 1) //vertical mirroring
        {
            if( (0x2800 <= Addr) && (Addr <= 0x2BFF) )
            {
                Addr &= 0x23FF;
            }
            else if( (0x2C00 <= Addr) && (Addr <= 0x2FFF) )
            {
                Addr &= 0x27FF;
            }
        }
        else //horizontal mirroring
        {
            if( (0x2400 <= Addr) && (Addr <= 0x27FF) )
            {
                Addr &= 0x23FF;
            }
            else if( (0x2C00 <= Addr) && (Addr <= 0x2FFF) )
            {
                Addr &= 0x2BFF;
            }
        }
    }
    else if( ( 0x3F00 <= Addr ) && ( Addr <= 0x3FFF ) )
    {
        Addr &= 0x3F1F;

        if(Addr == 0x3F10)
        {
            Addr = 0x3F00;
        }
        else if(Addr == 0x3F14)
        {
            Addr = 0x3F04;
        }
        else if(Addr == 0x3F18)
        {
            Addr = 0x3F08;
        }
        else if(Addr == 0x3F1C)
        {
            Addr = 0x3F0C;
        }
    }
}

void Bus::IncrementPPUADDR(PPU& ppu)
{
    if((ppu.PPUCTRL & 0b100) == 0)
    {
        ppu.V += 1; //add 1, going across.
    }
    else
    {
        ppu.V += 32; //add 32, going down
    }
}

void Bus::PollController()
{
    const std::uint8_t* CurrentKeyStates = SDL_GetKeyboardState(NULL);
    ControllerP1Reg =
    ( CurrentKeyStates[ SDL_SCANCODE_A ]    << 0 ) | ( CurrentKeyStates[ SDL_SCANCODE_B ]     << 1 ) |
    ( CurrentKeyStates[ SDL_SCANCODE_1 ]    << 2 ) | ( CurrentKeyStates[ SDL_SCANCODE_2 ]     << 3 ) |
    ( CurrentKeyStates[ SDL_SCANCODE_UP ]   << 4 ) | ( CurrentKeyStates[ SDL_SCANCODE_DOWN ]  << 5 ) |
    ( CurrentKeyStates[ SDL_SCANCODE_LEFT ] << 6 ) | ( CurrentKeyStates[ SDL_SCANCODE_RIGHT ] << 7 );
}

//updating only those registers that the ppu can write to.
void Bus::UpdateMMIO(PPU& ppu)
{
    CPUMemory[ppu.PPUSTATUSAddr] = ppu.PPUSTATUS;
    CPUMemory[ppu.OAMDATAAddr]   = ppu.OAMDATA;
    CPUMemory[ppu.PPUDATAAddr]   = ppu.PPUDATABuffer;
}

//H flip LUT (call when loading cartridge into memory).
std::uint8_t Bus::ReverseByte(std::uint8_t Byte)
{
    static const std::uint8_t ReverseByteLUT[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
    };
    return ReverseByteLUT[Byte];
}

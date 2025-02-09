#include "../src/bus.h"
#include "../src/ppu.h"

void PPU::Reset()
{
    V = T = X = W  = 0;
    PPUCTRL        = 0b00000000;
    PPUMASK        = 0b00000000;
    PPUSTATUS      = 0b10000000;
    OAMADDR        = 0x0000;
    OAMDATA        = 0x0000;
    PPUSCROLL      = 0x0000;
    PPUDATA        = 0x0000;
    PPUDATABuffer  = 0x00;
    OAMDMA         = 0x0000;
    PPUSCROLLX     = 0x0000;
    PPUSCROLLY     = 0x0000;
    Scanline       = 0;
    Cycle          = 0;
    ShifterTracker = 0;
    ShiftRegAttrLo = 0;
    ShiftRegAttrHi = 0;
    ShiftRegTileLo = 0;
    ShiftRegTileHi = 0;

    for(std::uint8_t i = 0; i<8; i++)
    {
        ShiftRegSpriteLo[i] = 0;
        ShiftRegSpriteHi[i] = 0;
    }

    LatchNTByte    = 0;
    LatchAttr      = 0;
    LatchTileLo    = 0;
    LatchTileHi    = 0;

    OAM_n    = 0; //sprite index in OAM
    OAM_m    = 0; //byte number for each sprite in OAM
    NSprites = 0; //tracking the number of sprites in each frame

    BGPixelVal    = 0;
    BGPalette     = 0;
    FGPixelVal    = 0;
    FGPriority    = 0;
    FGPalette     = 0;
    FinalPixelVal = 0;
    FinalPalette  = 0;

    VPrev = 0; //used to determine the correct attribute byte

    Sprite0HitPossible   = false;
    Sprite0BeingRendered = false;
    Sprite0HitDetected   = false;
    RenderBackground     = true;
    RenderSprites        = true;
    NMI                  = false;
    OddFrame             = false;
    FrameComplete        = false;
}

//checking if the ppu shall set NMI
bool PPU::SetNMI(Bus& bus)
{
    if((bus.CPUMemory[PPUCTRLAddr] & bus.CPUMemory[PPUSTATUSAddr] & 0b10000000) != 0)
    {
        return true;
    }
    return false;
}

void PPU::DecodeCHRAddress(std::uint16_t Address)
{
    std::uint8_t RowNumber    = Address & 0b111;
    std::uint8_t BitPlane     = (Address & 0b1000) >> 3;
    std::uint8_t TileNumber   = (Address & 0b111111110000) >> 4;
    std::uint8_t PatternTable = (Address & 0b1000000000000) >> 12;
    printf("RowNumber: %d\n", RowNumber);
    printf("BitPlane: %d\n", BitPlane);
    printf("TileNumber: 0x%04x\n", TileNumber);
    printf("PatternTable: %d\n", PatternTable);
}

std::vector<std::uint8_t> PPU::PushPatternTablesToGUI(Bus& bus)
{
    std::vector<std::vector<std::uint8_t>> Colours = {{{0,0,0},{255,83,83},{200,0,45},{255,255,255}}};
    std::vector<std::uint8_t> Framebuffer(128*256*3,0);
    for (std::uint32_t Row = 0; Row < 256; Row++) {
        for (std::uint32_t Col = 0; Col < 128; Col++) {

            std::uint16_t Addr  = (Row / 8 * 0x100) + (Row % 8) + (Col / 8) * 0x10;

            std::uint8_t PixelVal = ((bus.PPUMemory[Addr] >> (7-(Col % 8))) & 1) + ((bus.PPUMemory[Addr + 8] >> (7-(Col % 8))) & 1) * 2;

            Framebuffer[(Row * 128 * 3) + (Col * 3)]     = Colours[PixelVal][0]; //R value
            Framebuffer[(Row * 128 * 3) + (Col * 3) + 1] = Colours[PixelVal][1]; //G value
            Framebuffer[(Row * 128 * 3) + (Col * 3) + 2] = Colours[PixelVal][2]; //B value
        }
    }
    return Framebuffer;
}

void PPU::FetchLatchNTByte(Bus& bus)
{
    //getting the address of the tile in the nametable
    std::uint16_t TileAddr = 0x2000 | (V & 0x0FFF); //extracting bits 11-0.
    bus.MirrorPPUAddr(TileAddr);              //mirroring to the correct range
    LatchNTByte = bus.PPUMemory[TileAddr];
}

void PPU::FetchLatchAttrByte(Bus& bus)
{
    std::uint16_t AttrAddr = ( 0x23C0 | ( V & 0x0C00 ) | ( (V >> 4) & 0x0038 ) | ( (V >> 2) & 0x0007 ) );
    bus.MirrorPPUAddr(AttrAddr); //mirroring to the correct range
    LatchAttr = bus.PPUMemory[AttrAddr];
}

void PPU::FetchLatchTileLo(Bus& bus)
{
    std::uint8_t  FineY = (V & 0b111000000000000) >> 12;
    std::uint16_t PatternTableTileAddr = ( (PPUCTRL & 0b10000 ) ? 0x1000 : 0 ) + (LatchNTByte << 4);
    LatchTileLo    = bus.PPUMemory[(PatternTableTileAddr) + 0 + FineY];
    LatchTileLo    = bus.ReverseByte(LatchTileLo);
}

void PPU::FetchLatchTileHi(Bus& bus)
{
    std::uint8_t  FineY = (V & 0b111000000000000) >> 12;
    std::uint16_t PatternTableTileAddr = ( (PPUCTRL & 0b10000 ) ? 0x1000 : 0 ) + (LatchNTByte << 4);
    LatchTileHi    = bus.PPUMemory[(PatternTableTileAddr) + 8 + FineY];
    LatchTileHi    = bus.ReverseByte(LatchTileHi);

}

void PPU::IncrementV_Hori()
{
    if(RenderBackground || RenderSprites)
    {
        if ((V & 0x001F) == 31)    // if coarse X == 31
        {
            V &= ~0x001F;          // coarse X = 0
            V ^=  0x0400;          // switch horizontal nametable by flipping the bit
        }
        else
        {
            V++;                // increment coarse X
        }
    }
}

void PPU::IncrementV_Vert()
{
    if(RenderBackground || RenderSprites)
    {
        if ((V & 0x7000) != 0x7000) // if fine Y < 7
        {
            V += 0x1000;            // increment fine Y
        }
        else                        // gone beyond the height of a row. need to inc the row, potentially going into the adjacent nametable
        {
            V &= ~0x7000;           // fine Y = 0

            std::uint32_t Y = ( (V & 0x03E0) >> 5 ); // let y = coarse Y

            if(Y == 29)
            {
                Y = 0;                    // coarse Y = 0
                V ^= 0x0800;              // switch vertical nametable
            }
            else if(Y == 31)
            {
                Y = 0;                    // coarse Y = 0, nametable not switched
            }
            else
            {
                Y++;                      // increment coarse Y
            }
            V = (V & ~0x03E0) | (Y << 5); // put coarse Y back into v
        }
    }
}

void PPU::LoadBGShifters()
{
    ShiftRegTileLo = ( ShiftRegTileLo & 0xFF ) | (LatchTileLo << 8);
    ShiftRegTileHi = ( ShiftRegTileHi & 0xFF ) | (LatchTileHi << 8);

    std::uint16_t CoarseXBit1 = (VPrev & 0b0000010) >> 1;
    std::uint16_t CoarseYBit1 = (VPrev & 0b1000000) >> 6;

    std::uint8_t LatchAttrLoBit = 0;
    std::uint8_t LatchAttrHiBit = 0;

    if( (CoarseXBit1 == 0) && (CoarseYBit1 == 0) )
    {
        LatchAttrLoBit = (LatchAttr & 0b00000001) >> 0; //bit 0
        LatchAttrHiBit = (LatchAttr & 0b00000010) >> 1; //bit 1
    }
    else if( (CoarseXBit1 == 1) && (CoarseYBit1 == 0) )
    {
        LatchAttrLoBit = (LatchAttr & 0b00000100) >> 2; //bit 2
        LatchAttrHiBit = (LatchAttr & 0b00001000) >> 3; //bit 3
    }
    else if( (CoarseXBit1 == 0) && (CoarseYBit1 == 1) )
    {
        LatchAttrLoBit = (LatchAttr & 0b00010000) >> 4; //bit 4
        LatchAttrHiBit = (LatchAttr & 0b00100000) >> 5; //bit 5
    }
    else if( (CoarseXBit1 == 1) && (CoarseYBit1 == 1) )
    {
        LatchAttrLoBit = (LatchAttr & 0b01000000) >> 6; //bit 6
        LatchAttrHiBit = (LatchAttr & 0b10000000) >> 7; //bit 7
    }

    //making pseudo attribute shift registers, filled with 8 x the same value
    ShiftRegAttrLo = ( ShiftRegAttrLo & 0xFF ) | ( LatchAttrLoBit ? 0xFF00 : 0x0000 );
    ShiftRegAttrHi = ( ShiftRegAttrHi & 0xFF ) | ( LatchAttrHiBit ? 0xFF00 : 0x0000 );
}

void PPU::LoadFGShifters(Bus& bus)
{

    std::uint8_t  CoarseY = ( V & 0b000001111100000 ) >> 5;
    std::uint8_t  FineY   = ( V & 0b111000000000000 ) >> 12;
    std::uint16_t YPosn   = ( CoarseY << 3 ) | FineY;
    std::uint8_t  SpriteHeight  = ( ( PPUCTRL & 0b00100000 ) ? 16 : 8 );
    // printf("SpriteHeight: %02d\n", SpriteHeight);

    for(std::uint8_t i = 0; i < NSprites; i++)
    {
        ShiftRegSpriteLo[i] = 0;
        ShiftRegSpriteHi[i] = 0;

        std::int16_t  DeltaY = static_cast<std::int16_t>(Scanline+1) - static_cast<std::int16_t>( bus.OAMSecondary[ 4 * i ] + 1 );
        std::uint8_t  SpriteHeightOffset = ( (DeltaY >= 8) ? 1 : 0 );
        std::uint16_t PatternTableTileAddr = 0;

        DeltaY &= 0x07; //cropping back to 0-7 because we've already accounted for moving to the next tile

        if(SpriteHeight == 8)
        {
            //8 x 8 mode
            PatternTableTileAddr = ( (PPUCTRL & 0b1000) ? 0x1000 : 0x0000 ) | ( bus.OAMSecondary[ (4 * i) + 1 ] << 4 );
        }
        else
        {
            //8 x 16 mode
            std::uint16_t PatternTableTileBaseAddr = ( (bus.OAMSecondary[ (4 * i) + 1 ] & 1) ? 0x1000 : 0x0000 );
            std::uint16_t PatternTableTileNr = ( bus.OAMSecondary[ (4 * i) + 1 ] & 0xFE ) + SpriteHeightOffset;
            PatternTableTileAddr = PatternTableTileBaseAddr | (PatternTableTileNr << 4);
        }

        //determining if a sprite needs flipping vertically
        if( ( bus.OAMSecondary[ (4 * i) + 2 ] & 0b10000000 ) == 0 )
        {
            //no flip
            ShiftRegSpriteLo[i] = bus.PPUMemory[PatternTableTileAddr + 0 + DeltaY];
            ShiftRegSpriteHi[i] = bus.PPUMemory[PatternTableTileAddr + 8 + DeltaY];
        }
        else
        {
            //vertical flip
            ShiftRegSpriteLo[i] = bus.PPUMemory[ PatternTableTileAddr + ( (SpriteHeight - 1) - DeltaY) + 0 ];
            ShiftRegSpriteHi[i] = bus.PPUMemory[ PatternTableTileAddr + ( (SpriteHeight - 1) - DeltaY) + 8 ];
        }

        //determining if a sprite needs flipping horizontally
        if( ( bus.OAMSecondary[ (4 * i) + 2 ] & 0b01000000 ) == 0 )
        {
            //if not, then we need to reverse the bytes due to the msb being the first pixel
            ShiftRegSpriteLo[i] = bus.ReverseByte(ShiftRegSpriteLo[i]);
            ShiftRegSpriteHi[i] = bus.ReverseByte(ShiftRegSpriteHi[i]);
        }
    }
}

void PPU::UpdateAddrVReg_X()
{
    //copy all bits related to horizontal position from T to V
    if(RenderBackground || RenderSprites)
    {
        V = (V & 0b111101111100000) | (T & ~0b111101111100000);
    }
}

void PPU::UpdateAddrVReg_Y()
{
    //copy all bits related to vertical position from T to V
    if(RenderBackground || RenderSprites)
    {
        V = (V & 0b000010000011111) | (T & ~0b000010000011111);
    }
}

void PPU::UpdateBGShifters()
{
    if(RenderBackground)
    {
        ShiftRegTileLo >>= 1;
        ShiftRegTileHi >>= 1;
        ShiftRegAttrLo >>= 1;
        ShiftRegAttrHi >>= 1;
    }
}

void PPU::EvaluateSprites(Bus& bus)
{
    //setting all of secondary OAM to 0xFF in one go. Not accurate, but tbc if this can work.
    for(std::uint8_t i = 0; i<32; i++)
    {
        bus.OAMSecondary[i] = 0xFF;
    }

    OAM_n      = 0;
    OAM_m      = 0;
    NSprites   = 0;
    PPUSTATUS &= 0b11011111; //clearing sprite overflow
    Sprite0HitPossible = false;

    std::uint8_t  SpriteHeight = ( ( PPUCTRL & 0b00100000 ) ? 16 : 8 );
    std::uint8_t  CoarseY      = ( V & 0b000001111100000 ) >> 5;
    std::uint8_t  FineY        = ( V & 0b111000000000000 ) >> 12;
    std::uint16_t YPosn        = ( CoarseY << 3 ) | FineY;

    //Sprite evaluation for next scanline
    while( !( (PPUSTATUS & 0b00100000) >> 5 ) && (OAM_n <= 63) )
    {
        //checking if Y is in range
        std::int16_t DeltaY = static_cast<std::int16_t>(Scanline+1) - static_cast<std::int16_t>( bus.OAM[ 4 * OAM_n ] + 1 );

        //checking if we will collide with this sprite on this scanline
        if( ( DeltaY >= 0 ) && ( DeltaY < SpriteHeight ) )
        {
            //checking to see if we have collided with sprite 0 on this line, and we haven't already had a 0-hit this frame
            if( (OAM_n == 0) && !(Sprite0HitDetected))
            {
                //it is possible that this scanline will have a sprite-0 hit
                Sprite0HitPossible = true;
            }
            if(NSprites == 8)
            {
                //setting sprite overflow flag
                PPUSTATUS |= 0b00100000;
            }
            else
            {
                bus.OAMSecondary[ (4 * NSprites) + 0 ] = bus.OAM[ (4 * OAM_n) + 0 ];
                bus.OAMSecondary[ (4 * NSprites) + 1 ] = bus.OAM[ (4 * OAM_n) + 1 ];
                bus.OAMSecondary[ (4 * NSprites) + 2 ] = bus.OAM[ (4 * OAM_n) + 2 ];
                bus.OAMSecondary[ (4 * NSprites) + 3 ] = bus.OAM[ (4 * OAM_n) + 3 ];
                NSprites++;
            }
        }
        OAM_n++;
    }
}

void PPU::EvaluateBGPixel(Bus& bus)
{
    BGPixelVal = BGPalette = 0;

    std::uint32_t FramebufferID = (Scanline * 256 * 3) + ( (Cycle - 2) * 3 );

    //getting the appropriate mask to select the bit defined by fine-x
    std::uint16_t BitMux = ( 1 << X );

    //PixelVal shall be a number between 0 & 3 to choose a colour from the palette
    std::uint8_t PixelValLo = ( ShiftRegTileLo & BitMux ) >> X;
    std::uint8_t PixelValHi = ( ShiftRegTileHi & BitMux ) >> X;
    std::uint8_t PixelVal   = ( PixelValHi << 1 ) | PixelValLo;

    //getting the 3-bit value of the palette to select from for the BG
    std::uint8_t AttrValLo = ( ShiftRegAttrLo & BitMux ) >> X;
    std::uint8_t AttrValHi = ( ShiftRegAttrHi & BitMux ) >> X;

    std::uint8_t AttrVal   = ( AttrValHi << 1 ) | AttrValLo;
    if ( (PixelVal != 0) && RenderBackground )
    {
        BGPixelVal = PixelVal;
        BGPalette  = AttrVal;
    }
}

void PPU::EvaluateFGPixel(Bus& bus)
{
    FGPixelVal = FGPalette = FGPriority = 0;
    Sprite0BeingRendered = false;
    std::uint32_t FramebufferID = (Scanline * 256 * 3) + ( (Cycle - 2) * 3 );
    std::uint8_t  SpriteHeight  = ( ( PPUCTRL & 0b00100000 ) ? 16 : 8 );

    //loop through all 8 entries in OAMSecondary
    for(std::int8_t i = NSprites - 1; i >= 0; i--)
    {
        //checking to see if we have collided with a sprite
        std::int16_t DeltaX = static_cast<std::int16_t>(Cycle - 2) - static_cast<std::int16_t>( bus.OAMSecondary[ (4 * i) + 3 ] );

        if( ( DeltaX >= 0 ) && ( DeltaX < 8 ) )
        {
            //PixelVal shall be a number between 0 & 3 to choose a colour from the palette
            std::uint8_t PixelValLo = ( ShiftRegSpriteLo[i] & 1 );
            std::uint8_t PixelValHi = ( ShiftRegSpriteHi[i] & 1 );
            std::uint8_t PixelVal   = ( PixelValHi << 1 ) | PixelValLo;
            std::uint8_t AttrVal    = bus.OAMSecondary[ (4 * i) + 2 ] & 0b11;

            //We will only render a sprite pixel if it is non-transparent & rendering sprites enabled
            if(PixelVal != 0 && RenderSprites)
            {
                FGPixelVal = PixelVal;
                FGPalette  = AttrVal + 4;
                FGPriority = bus.OAMSecondary[ (4 * i) + 2 ] & 0b00100000;
            }

            //we have collided with sprite 0 && the pixel is visible && we haven't already had a 0-hit this frame
            if( (i == 0) && (PixelVal != 0) )
            {
                Sprite0BeingRendered = true;
            }

            ShiftRegSpriteLo[i] >>= 1;
            ShiftRegSpriteHi[i] >>= 1;
        }
    }
}

void PPU::RenderPixel(Bus& bus)
{
    FinalPixelVal = FinalPalette = 0;

    std::uint32_t FramebufferID = (Scanline * 256 * 3) + ( (Cycle - 2) * 3 );

    if( (BGPixelVal == 0) && (FGPixelVal == 0) )
    {
        FinalPixelVal = 0x00;
        FinalPalette  = 0x00;
    }
    else if( (BGPixelVal == 0) && (FGPixelVal > 0) )
    {
        FinalPixelVal = FGPixelVal;
        FinalPalette  = FGPalette;
    }
    else if( (BGPixelVal > 0) && (FGPixelVal == 0) )
    {
        FinalPixelVal = BGPixelVal;
        FinalPalette  = BGPalette;
    }
    else if( (BGPixelVal > 0) && (FGPixelVal > 0) )
    {
        if( FGPriority != 0 )
        {
            FinalPixelVal = BGPixelVal;
            FinalPalette  = BGPalette;
        }
        else
        {
            FinalPixelVal = FGPixelVal;
            FinalPalette  = FGPalette;
        }
    }

    if(Sprite0HitPossible && Sprite0BeingRendered)
    {
        if(RenderBackground && RenderSprites)
        {
            Sprite0HitDetected = true;
            PPUSTATUS |= 0b01000000;
        }
    }

    std::uint16_t PaletteAddr = 0x3F00 + (4 * FinalPalette) + FinalPixelVal;
    std::uint8_t  ColourID = bus.PPUMemory[ PaletteAddr ];

    for(std::uint8_t i = 0; i<3; i++)
    {
        Framebuffer[ FramebufferID + i ] = ColourPalette[3*ColourID + i];     //R value
    }

}

void PPU::Clock(Bus& bus)
{
    if(0 <= Scanline && Scanline <= 239) //visible scanlines
    {
        if (Cycle == 0) //idle cycle
        {
            Cycle++;
        }
        else if(Cycle == 1) //fetch the first NT byte. Separated from the below because the shifters only shift from the 2nd cycle...
        {
            FetchLatchNTByte(bus);
            Cycle++;
        }
        else if( ( (2 <= Cycle) && (Cycle <= 257) ) || ( (321 <= Cycle) && (Cycle <= 337) ) )
        {
            //First thing we do in each cycle is render a pixel.
            if( (2 <= Cycle) && (Cycle <= 257) )
            {
                EvaluateBGPixel(bus);
                EvaluateFGPixel(bus);
                RenderPixel(bus);
            }

            //Then we shift the registers
            UpdateBGShifters();

            switch( Cycle % 8 )
            {
                case 0:
                {
                    VPrev = V;
                    IncrementV_Hori();
                }break;
                case 1:
                {
                    LoadBGShifters();
                    FetchLatchNTByte(bus);
                }break;
                case 3:
                {
                    FetchLatchAttrByte(bus);
                }break;
                case 5:
                {
                    FetchLatchTileLo(bus);
                }break;
                case 7:
                {
                    FetchLatchTileHi(bus);
                }break;
            }
            if(Cycle == 256) //we have reached the end of a scanline. increment vertical position.
            {
                IncrementV_Vert();
            }
            else if(Cycle == 257)
            {
                UpdateAddrVReg_X();
                OAMADDR = 0;
                EvaluateSprites(bus);
            }
            Cycle++;
        }
        else if (258 <= Cycle && Cycle <= 320) //tbc if this is correct. cycle 257 or 258?
        {
            OAMADDR = 0;
            Cycle++;
        }
        else if (338 <= Cycle && Cycle <= 339)
        {
            Cycle++;
        }
        else if (Cycle == 340)
        {
            LoadFGShifters(bus);
            Cycle = 0;
            Scanline++;
        }
    }
    else if (Scanline == 240) //post-render idle scanline
    {
        if(Cycle == 340)
        {
            Cycle = 0;
            Scanline++;
        }
        else
        {
            Cycle++;
        }
    }
    else if (241 <= Scanline && Scanline <= 260) //vblank scanlines
    {
        if (Scanline == 241 && Cycle == 1)
        {
            PPUSTATUS |= 0b10000000; //setting vblank
            Cycle++;
        }
        else if (Cycle == 257)
        {
            Cycle++;
        }
        else if (Cycle == 340)
        {
            Cycle = 0;
            Scanline++;
        }
        else
        {
            Cycle++;
        }
    }
    else //pre-render scanline, #261
    {
        if (Cycle == 0) //idle cycle
        {
            Cycle++;
        }
        else if(Cycle == 1)
        {
            PPUSTATUS &= 0b00111111;    //clearing vblank and sprite 0 hit
            Sprite0HitDetected = false;
            FetchLatchNTByte(bus);
            Cycle++;
        }
        else if( (2 <= Cycle && Cycle <= 257) || (321 <= Cycle && Cycle <= 337) )
        {
            UpdateBGShifters();

            switch( Cycle % 8 )
            {
                case 0:
                {
                    VPrev = V;
                    IncrementV_Hori();
                }break;
                case 1:
                {
                    LoadBGShifters();
                    FetchLatchNTByte(bus);
                }break;
                case 3:
                {
                    FetchLatchAttrByte(bus);
                }break;
                case 5:
                {
                    FetchLatchTileLo(bus);
                }break;
                case 7:
                {
                    FetchLatchTileHi(bus);
                }break;
            }
            if(Cycle == 256) //we have reached the end of a scanline. increment vertical position.
            {
                IncrementV_Vert();
            }
            else if(Cycle == 257)
            {
                UpdateAddrVReg_X();
                OAMADDR = 0;
            }
            Cycle++;
        }
        else if(258 <= Cycle && Cycle <= 279)
        {
            OAMADDR = 0;
            Cycle++;
        }
        else if( (280 <= Cycle) && (Cycle <= 304) )
        {
            UpdateAddrVReg_Y();
            Cycle++;
        }
        else if(305 <= Cycle && Cycle <= 320)
        {
            Cycle++;
        }
        else if(338 <= Cycle && Cycle <= 338)
        {
            Cycle++;
        }
        else if( (Cycle == 339) && OddFrame )
        {
            Cycle         = 0;
            Scanline      = 0;
            OddFrame      = false;
            FrameComplete = true;
        }
        else if( (Cycle == 339) && !OddFrame )
        {
            Cycle++;
        }
        else if(Cycle == 340)
        {
            Cycle         = 0;
            Scanline      = 0;
            OddFrame      = true;
            FrameComplete = true;
        }
    }
}

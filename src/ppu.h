#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <vector>

struct Bus;
struct Mem;

struct PPU
{
    //INTERNAL REGISTERS

    //INTERNAL REGISTERS
    std::uint16_t V;
    std::uint16_t T;
    std::uint8_t  X;
    std::uint8_t  W; //PPUADDRLatch

    const std::uint16_t PPUCTRLAddr   = 0x2000;
    const std::uint16_t PPUMASKAddr   = 0x2001;
    const std::uint16_t PPUSTATUSAddr = 0x2002;
    const std::uint16_t OAMADDRAddr   = 0x2003;
    const std::uint16_t OAMDATAAddr   = 0x2004;
    const std::uint16_t PPUSCROLLAddr = 0x2005;
    const std::uint16_t PPUADDRAddr   = 0x2006;
    const std::uint16_t PPUDATAAddr   = 0x2007;
    const std::uint16_t OAMDMAAddr    = 0x4014;

    std::uint16_t PPUCTRL;
    std::uint16_t PPUMASK;
    std::uint16_t PPUSTATUS;
    std::uint16_t OAMADDR;
    std::uint16_t OAMDATA;
    std::uint16_t PPUSCROLL;
    std::uint16_t PPUDATA;
    std::uint8_t  PPUDATABuffer;
    std::uint16_t OAMDMA;
    std::uint16_t PPUSCROLLX;
    std::uint16_t PPUSCROLLY;

    std::uint16_t Scanline;
    std::uint16_t Cycle;

    std::uint16_t ShifterTracker;

    std::uint16_t ShiftRegAttrLo;
    std::uint16_t ShiftRegAttrHi;
    std::uint16_t ShiftRegTileLo;
    std::uint16_t ShiftRegTileHi;

    std::uint8_t ShiftRegSpriteLo[8];
    std::uint8_t ShiftRegSpriteHi[8];

    std::uint8_t  LatchNTByte;
    std::uint8_t  LatchAttr;
    std::uint8_t  LatchTileLo;
    std::uint8_t  LatchTileHi;

    std::uint8_t OAM_n; //sprite index in OAM
    std::uint8_t OAM_m; //byte number for each sprite in OAM
    std::uint8_t NSprites; //tracking the number of sprites in each frame

    std::uint8_t BGPixelVal;
    std::uint8_t BGPalette;
    std::uint8_t FGPixelVal;
    std::uint8_t FGPriority;
    std::uint8_t FGPalette;
    std::uint8_t FinalPixelVal;
    std::uint8_t FinalPalette;

    std::uint16_t VPrev; //used to determine the correct attribute byte

    bool Sprite0HitPossible;
    bool Sprite0BeingRendered;
    bool Sprite0HitDetected;
    bool RenderBackground;
    bool RenderSprites;
    bool NMI;
    bool OddFrame; //this is to determine the length of the pre-render scanline. TBC what this is initialised as
    bool FrameComplete;

    std::vector<std::uint8_t> Framebuffer{std::vector<std::uint8_t>(256*240*3,0)};

    void Reset();

    bool SetNMI(Bus&);

    void DecodeCHRAddress(std::uint16_t);

    std::vector<std::uint8_t> PushPatternTablesToGUI(Bus&);

    void FetchLatchNTByte(Bus&);

    void FetchLatchAttrByte(Bus&);

    void FetchLatchTileLo(Bus&);

    void FetchLatchTileHi(Bus&);

    void LoadBGShifters();

    void LoadFGShifters(Bus&);

    void IncrementV_Hori();

    void IncrementV_Vert();

    void UpdateAddrVReg_X();

    void UpdateAddrVReg_Y();

    void UpdateBGShifters();

    void EvaluateSprites(Bus&);

    void EvaluateBGPixel(Bus&);

    void EvaluateFGPixel(Bus&);

    void RenderPixel(Bus& bus);

    void Clock(Bus&);

    const std::vector<std::uint8_t> ColourPalette = {
    0x69, 0x69, 0x69, 0x00, 0x14, 0x8F, 0x1E, 0x02, 0x9B, 0x3F, 0x00, 0x8A, 0x60, 0x00, 0x60, 0x66,
    0x00, 0x17, 0x57, 0x0D, 0x00, 0x45, 0x1B, 0x00, 0x24, 0x34, 0x00, 0x00, 0x42, 0x00, 0x00, 0x45,
    0x00, 0x00, 0x3C, 0x1F, 0x00, 0x31, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xAF, 0xAF, 0xAF, 0x0F, 0x51, 0xDD, 0x44, 0x2F, 0xF3, 0x72, 0x20, 0xE2, 0xA3, 0x19, 0xB3, 0xAE,
    0x1C, 0x51, 0xA4, 0x34, 0x00, 0x88, 0x4D, 0x00, 0x67, 0x6D, 0x00, 0x20, 0x80, 0x00, 0x00, 0x8B,
    0x00, 0x00, 0x7F, 0x42, 0x00, 0x6C, 0x97, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0x65, 0xAA, 0xFF, 0x8C, 0x96, 0xFF, 0xB9, 0x83, 0xFF, 0xDD, 0x6F, 0xFF, 0xEA,
    0x6F, 0xBD, 0xEB, 0x84, 0x66, 0xDC, 0xA2, 0x1F, 0xBA, 0xB4, 0x03, 0x7E, 0xCB, 0x07, 0x54, 0xD3,
    0x3E, 0x3C, 0xD2, 0x84, 0x3E, 0xC7, 0xCC, 0x4B, 0x4B, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xBD, 0xE2, 0xFF, 0xCE, 0xCF, 0xFF, 0xE6, 0xC2, 0xFF, 0xF6, 0xBC, 0xFF, 0xF9,
    0xC2, 0xED, 0xFA, 0xCF, 0xC6, 0xF8, 0xDE, 0xAC, 0xEE, 0xE9, 0xA1, 0xD0, 0xF5, 0x9F, 0xBB, 0xF5,
    0xAF, 0xB3, 0xF5, 0xCD, 0xB9, 0xED, 0xF0, 0xB9, 0xB9, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const std::vector<std::uint8_t> AttrByteShiftLUT = {
    0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,
    0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,
    4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,
    4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6
    };

};

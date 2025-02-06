#pragma once
#include <cstdint>
#include <fstream>
#include <vector>
#include "../src/apu.h"
#include "../src/ppu.h"

struct APU; //forward declaring the apu struct

struct Bus
{

    APU* apu_ptr;

    PPU* ppu_ptr;

    bool AddressMirroring = true;

    std::uint16_t DMACycles         = 0;
    std::uint16_t OAMCount          = 0;

    std::uint8_t CPUMemory[65536]   = {0};

    std::uint8_t PPUMemory[16384]   = {0};

    std::uint8_t CHRROM[1024*200]   = {0};

    std::uint8_t PRGROM[1024*200]   = {0};

    std::uint8_t OAM[256]           = {0};

    std::uint8_t OAMSecondary[32]   = {0};

    std::uint8_t ControllerP1Reg    = 0x40;

    std::uint32_t PRGROMStartAddr   = 0;
    std::uint32_t CHRROMStartAddr   = 0;
    std::uint8_t  NametableMirror   = 0; //1 = vertical, 0 = horizontal
    std::uint8_t  Mapper            = 0;

    //controller button bit flags
    const std::uint8_t CONTROLLER_A       = 0b00000001;
    const std::uint8_t CONTROLLER_B       = 0b00000010;
    const std::uint8_t CONTROLLER_SELECT  = 0b00000100;
    const std::uint8_t CONTROLLER_START   = 0b00001000;
    const std::uint8_t CONTROLLER_UP      = 0b00010000;
    const std::uint8_t CONTROLLER_DOWN    = 0b00100000;
    const std::uint8_t CONTROLLER_LEFT    = 0b01000000;
    const std::uint8_t CONTROLLER_RIGHT   = 0b10000000;

    void ConnectAPU(APU&);
    void ConnectPPU(PPU&);

    void LoadCartridge(std::string);

    void SelectBank(std::uint8_t);

    void WriteCPUBus(std::uint8_t, std::uint16_t);

    std::uint8_t ReadCPUBus(std::uint16_t);

    void WritePPUBus(std::uint8_t, std::uint16_t);

    std::uint8_t ReadPPUBus(std::uint16_t);

    void WriteAPUBus(std::uint8_t, std::uint16_t);

    std::uint8_t ReadAPUBus(std::uint16_t);

    void IncrementPPUADDR();

    void MirrorPPUAddr(std::uint16_t&);

    void PollController();

    void UpdateMMIO();

    std::uint8_t ReverseByte(std::uint8_t);
};

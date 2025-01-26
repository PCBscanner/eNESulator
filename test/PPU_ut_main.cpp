#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <SDL2/SDL.h>
#include <string>
#include <tuple>
#include <vector>

#include "../deps/acutest.h" //https://github.com/mity/acutest
#include "../src/bus.h"
#include "../src/ppu.h"
#include "../src/6502.h"
#include "../src/memory.h"


void PPUCanDecodeTheBytePosition()
{

    Bus bus;
    Mem ppuMem;
    Mem cpuMem;
    CPU cpu;
    PPU ppu;
    ppu.DecodeCHRAddress(0x66FC);

    cpuMem.Initialise(1024*64);
    ppuMem.Initialise(1024*8);

    std::string ROM = "../data/roms/Donkey Kong (Japan).nes";
    bus.LoadCartridge(ROM, cpuMem);

    ppu.DisplayCHRTile(bus, 0x0000);
}

void PPUCanSetNMITrue()
{

    Bus bus;
    Mem ppuMem;
    Mem cpuMem;
    CPU cpu;
    PPU ppu;

    ppu.NMI = false; //initialising NMI as false
    ppu.Scanline = 241;
    ppu.Cycle = 1;
    cpuMem.Initialise(1024*64);
    ppuMem.Initialise(1024*8);
    cpuMem[0x2000] = 0b10000000; //cpu requesting Vblank
    cpuMem[0x2002] = 0b10000000; //ppu requesting Vblank

    ppu.Execute(cpuMem, bus);
    TEST_CHECK(ppu.NMI == true);

}

TEST_LIST = {
    {"PPUCanDecodeTheBytePosition", PPUCanDecodeTheBytePosition},
    {"PPUCanSetNMITrue", PPUCanSetNMITrue},
    { NULL, NULL }     /* zeroed record marking the end of the list */
};

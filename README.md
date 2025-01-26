# eNESulator
NES emulator written in C++

## Introduction
This is my version of an emulator for the Nintendo Entertainment System. I've used this as a (rather ambitious) way to learn C++.\

It is far from complete, but it has the following features:
- 1-player controls.
- Mappers: iNES 000 & 002.
- Horizontal and vertical scrolling.
- 8x16 sprites.

## Future improvements:
- Audio (APU emulation).
- Additional mappers.
- Battery-backed RAM.
- Save states.
- 2-player controls.

## Prerequisites:
SDL2 is used for handling the GUI and controller inputs.\

I have verified this is working with C++17 and C++20 on Fedora 41. Note that C++20 is only required if you want to use `std::format` to display hex numbers with the leading 0x on the GUI.

## Compiling, building, and running the emulator
`clear && g++ -std=c++20 -o ../bin/eNESulator ../src/main.cpp ../src/bus.cpp ../src/ppu.cpp -lSDL2 -lSDL2_ttf && ../bin/eNESulator`

## References:
There were many references that helped me through this project. Some notable ones are:
- OneLoneCoder's series on writing a NES emulator: https://github.com/OneLoneCoder/olcNES
- David Poo's series on writing a 6502 CPU emulator: https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37
- The entire Nesdev Wiki: https://www.nesdev.org/wiki/Nesdev_Wiki
- Famicom Party: https://famicom.party/
- Emulation Online: https://www.emulationonline.com/systems/nes/
- emudev: https://emudev.de/nes-emulator/overview/

## Disclaimer
This program is not endorsed by Nintendo.

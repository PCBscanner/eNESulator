# eNESulator
NES emulator written in C++

## Introduction
This is my version of an emulator for the Nintendo Entertainment System. I've used this as a (rather ambitious) way to learn C++.

It is still WIP, but has been tested and confirmed running with the following roms:

- '1942.
- Castlevania.
- Donkey Kong.
- Excitebike.
- Ice Climber.
- nestest
- Super Mario Bros.


## Features:
- Official NES opcodes.
- 1-player controls.
- Mappers: iNES 000 & 002.
- Horizontal and vertical scrolling.
- 8x16 sprites.
- Basic APU implementation (DMC is not passing the various test ROMs).

## Future improvements:
- Fix DMC channel in the APU.
- Additional mappers.
- Battery-backed RAM.
- Save states.
- 2-player controls.
- Work through the various test roms and fix issues as required.


## Prerequisites:
SDL2 is used for handling the GUI and controller inputs.

I have verified this is working with C++17 and C++20 on Fedora 41. Note that C++20 is only required if you want to use `std::format` to display hex numbers with the leading 0x on the GUI.

## Compiling, building, and running the emulator
You can use GNU make from the src directorym with various options such as `make clean` or `make clean_objects`.

## References:
There were many references that helped me through this project. Some notable ones are:
- OneLoneCoder's series on writing a NES emulator: https://github.com/OneLoneCoder/olcNES
- David Poo's series on writing a 6502 CPU emulator: https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37
- The entire Nesdev Wiki: https://www.nesdev.org/wiki/Nesdev_Wiki
- Famicom Party: https://famicom.party/
- Emulation Online: https://www.emulationonline.com/systems/nes/
- emudev: https://emudev.de/nes-emulator/overview/
- nes-test-roms: https://github.com/christopherpow/nes-test-roms

## Disclaimer
This program is not endorsed by Nintendo.

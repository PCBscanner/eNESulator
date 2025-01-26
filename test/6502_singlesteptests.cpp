#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

#include "../deps/json.hpp"
#include "../deps/acutest.h" //https://github.com/mity/acutest
#include "../src/memory.h"
#include "../src/6502.h"

using json = nlohmann::json;

static const std::string TestFileDir = "/home/main/Documents/Coding/NES_emu/data/65x02/6502/v1/";
static const std::string TestFileSuffix = ".json";
static const std::vector<std::string> TestFiles = {
    "a9", "a5", "b5", "ad", "bd", "b9", "a1", "b1", //LDA
    "a2", "a6", "b6", "ae", "be",                   //LDX
    "a0", "a4", "b4", "ac", "bc",                   //LDY
    "85", "95", "8d", "9d", "99", "81", "91",       //STA
    "86", "96", "8e",                               //STX
    "84", "94", "8c",                               //STY
    "20",                                           //JSR
    "4c", "6c",                                     //JMP
    "60",                                           //RTS
    "ba",                                           //TSX
    "9a",                                           //TXS
    "48",                                           //PHA
    "08",                                           //PHP
    "68",                                           //PLA
    "28",                                           //PLP
    "29", "25", "35", "2d", "3d", "39", "21", "31", //AND
    "49", "45", "55", "4d", "5d", "59", "41", "51", //EOR
    "09", "05", "15", "0d", "1d", "19", "01", "11", //ORA
    "24", "2c",                                     //BIT
    "aa",                                           //TAX
    "a8",                                           //TAY
    "8a",                                           //TXA
    "98",                                           //TYA
    "e8",                                           //INX
    "c8",                                           //INY
    "ca",                                           //DEX
    "88",                                           //DEY
    "e6", "f6", "ee", "fe",                         //INC
    "c6", "d6", "ce", "de",                         //DEC
    "90",                                           //BCC
    "b0",                                           //BCS
    "f0",                                           //BEQ
    "d0",                                           //BNE
    "30",                                           //BMI
    "10",                                           //BPL
    "50",                                           //BVC
    "70",                                           //BVS
    "18",                                           //CLC
    "d8",                                           //CLD
    "58",                                           //CLI
    "b8",                                           //CLV
    "38",                                           //SEC
    "f8",                                           //SED
    "78",                                           //SEI
    "c9", "c5", "d5", "cd", "dd", "d9", "c1", "d1", //CMP
    "e0", "e4", "ec",                               //CPX
    "c0", "c4", "cc",                               //CPY
    "0a", "06", "16", "0e", "1e",                   //ASL
    "4a", "46", "56", "4e", "5e",                   //LSR
    "2a", "26", "36", "2e", "3e",                   //ROL
    "6a", "66", "76", "6e", "7e",                   //ROR
    "00",                                           //BRK
    "40"                                            //RTI
};

static const vector<string> TestFilesAddSubtract = {
    "69", "65", "75", "6d", "7d", "79", "61", "71", //ADC NEED TO IMPLEMENT DECIMAL MODE
    "e9", "e5", "f5", "ed", "fd", "f9", "e1", "f1" //SBC NEED TO IMPLEMENT DECIMAL MODE
};


void SingleStepTests()
{
    cout << endl;
    for (std::uint32_t n = 0; n<TestFiles.size(); n++) //looping over all specified test files
    {
        Mem mem;
        CPU cpu;
        Bus bus;
        PPU ppu;

        bus.AddressMirroring = false;

        string TestFilePath = TestFileDir + TestFiles[n] + TestFileSuffix;
        cout << TestFilePath << endl;

        json data = json::parse(ifstream(TestFilePath));

        for (std::uint32_t i = 0; i<data.size(); i++)
        {
            std::uint32_t Cycles = cpu.Reset(bus, ppu);
            //name of test
            string TestName = data[i]["name"];
            //initial values for test
            cpu.A  = data[i]["initial"]["a"];
            cpu.X  = data[i]["initial"]["x"];
            cpu.Y  = data[i]["initial"]["y"];
            cpu.PC = data[i]["initial"]["pc"];
            cpu.PS = data[i]["initial"]["p"];
            cpu.SP = data[i]["initial"]["s"];

            //looping over all memory values
            for(std::uint32_t j = 0; j<data[i]["initial"]["ram"].size(); j++)
            {
                bus.CPUMemory[data[i]["initial"]["ram"][j][0].get<std::uint32_t>()] = data[i]["initial"]["ram"][j][1];
            }
            cpu.Execute(Cycles, bus, ppu);
            //expected final values from test test
            TEST_CHECK(cpu.A  == data[i]["final"]["a"]);
            TEST_CHECK(cpu.X  == data[i]["final"]["x"]);
            TEST_CHECK(cpu.Y  == data[i]["final"]["y"]);
            TEST_CHECK(cpu.PC == data[i]["final"]["pc"]);
            TEST_CHECK(cpu.PS == data[i]["final"]["p"]);
            TEST_CHECK(cpu.SP == data[i]["final"]["s"]);

            //looping over all memory values
            for(std::uint32_t j = 0; j<data[i]["final"]["ram"].size(); j++)
            {
                TEST_CHECK(bus.CPUMemory[data[i]["final"]["ram"][j][0].get<std::uint32_t>()] == data[i]["final"]["ram"][j][1]);
            }
        }
    }
}

void SingleStepTestsAddSubtract()
{
    cout << endl;
    for (std::uint32_t n = 0; n<TestFilesAddSubtract.size(); n++) //looping over all specified test files
    {
        Mem mem;
        CPU cpu;
        Bus bus;
        PPU ppu;

        bus.AddressMirroring = false;

        string TestFilePath = TestFileDir + TestFilesAddSubtract[n] + TestFileSuffix;
        cout << TestFilePath << endl;

        json data = json::parse(ifstream(TestFilePath));

        for (std::uint32_t i = 0; i<data.size(); i++)
        {
            // printf("i: %d\n", i);
            if ((data[i]["initial"]["p"].get<std::uint8_t>() & 0b00001000) != 0)
            // if (false)
                {
                // printf("BCD mode not yet implemented - skipping.\n");
                continue;
            }
            else
            {
                std::uint32_t Cycles = cpu.Reset(bus, ppu);
                //name of test
                // string TestName = data[i]["name"];
                //initial values for test
                cpu.A  = data[i]["initial"]["a"];
                cpu.X  = data[i]["initial"]["x"];
                cpu.Y  = data[i]["initial"]["y"];
                cpu.PC = data[i]["initial"]["pc"];
                cpu.PS = data[i]["initial"]["p"];
                cpu.SP = data[i]["initial"]["s"];

                //looping over all memory values
                for(std::uint32_t j = 0; j<data[i]["initial"]["ram"].size(); j++)
                {
                    bus.CPUMemory[data[i]["initial"]["ram"][j][0].get<std::uint32_t>()] = data[i]["initial"]["ram"][j][1];
                }
                cpu.Execute(Cycles, bus, ppu);
                //expected final values from test test
                TEST_CHECK(cpu.A  == data[i]["final"]["a"]);
                TEST_CHECK(cpu.X  == data[i]["final"]["x"]);
                TEST_CHECK(cpu.Y  == data[i]["final"]["y"]);
                TEST_CHECK(cpu.PC == data[i]["final"]["pc"]);
                TEST_CHECK(cpu.PS == data[i]["final"]["p"]);
                TEST_CHECK(cpu.SP == data[i]["final"]["s"]);

                //looping over all memory values
                for(std::uint32_t j = 0; j<data[i]["final"]["ram"].size(); j++)
                {
                    TEST_CHECK(bus.CPUMemory[data[i]["final"]["ram"][j][0].get<std::uint32_t>()] == data[i]["final"]["ram"][j][1]);
                }
                // printf("i: %d\n", i);
            }
        }

    }
}

TEST_LIST = {
    {"SingleStepTests", SingleStepTests},
    // {"SingleStepTestsAddSubtract", SingleStepTestsAddSubtract},
    { NULL, NULL }     /* zeroed record marking the end of the list */
};

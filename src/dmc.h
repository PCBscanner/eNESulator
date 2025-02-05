#pragma once

#include <cstdint>

struct DMC
{

    //$4010
    std::uint8_t IRQEnabledFlag = 0; //if clear, the interrupt flag is cleared
    std::uint8_t LoopFlag       = 0;
    std::uint8_t RateIndex      = 0; //this is the rate in CPU cycles, NOT APU cycles!
    
    //$4011
    std::uint8_t DirectLoad     = 0;
    
    //$4012
    std::uint16_t SampleAddr    = 0;
    
    //$4013
    std::uint16_t SampleLength  = 0;
    
    //other parameters...
    std::uint8_t  IRQFlag        = 0;
    std::uint8_t  SampleBuffer   = 0;
    std::uint8_t  SampleBufferNext = 0; //workaround to accessing the bus memory from this struct
    std::uint16_t Timer          = 0;
    std::uint8_t  ShiftRegister  = 0;
    std::uint8_t  BitsRemaining  = 0;
    std::uint8_t  SilenceFlag    = 0;
    std::uint8_t  OutputLevel    = 0;
    std::uint16_t CurrentAddr    = 0;
    std::uint16_t BytesRemaining = 0;
    std::uint8_t  Enabled        = 0;
    
    std::uint8_t Clock();
    void ClockTimer();
    void UpdateOutputLevel();
    void ClockShiftRegister();
    void UpdateBitsRemaining();
    void StartNewOutputCycle();
    void EmptySampleBuffer();
    void RefillSampleBuffer();
    void DecrementBytesRemaining();  
    
    std::uint16_t RateLUT[16] = {428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54};
};

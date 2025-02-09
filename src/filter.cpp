#include "../src/filter.h"

float FIRFilter::Update(float Input)
{
    //store latest sample into the buffer
    Buffer[BufferIndex] = Input;
    //increment buffer and wrap around if necessary
    if(BufferIndex == FilterLength-1)
    {
        //wrap around back to 0
        BufferIndex = 0;
    }
    else
    {
        //increment as normal
        BufferIndex++;
    }
    //reset output to 0 before calculating new output
    Output = 0;

    std::uint8_t SumIndex = BufferIndex;

    for(std::uint8_t n = 0; n<FilterLength; n++)
    {
        //decrement index and wrap if necessary
        if(SumIndex > 0)
        {
            SumIndex--;
        }
        else
        {
            SumIndex = FilterLength - 1;
        }
        //multiply impulse response with shifted input sample, and add to the output
        Output += FIRImpulseResponse[n] * Buffer[SumIndex];
    }
    return Output;
}

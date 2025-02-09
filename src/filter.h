#pragma once

#include <cstdint>

struct FIRFilter
{
    static const std::uint16_t FilterLength = 120;
    float Buffer[FilterLength] = {0};
    std::uint16_t BufferIndex = 0;
    float Output = 0;

    //http://t-filter.engineerjs.com/
    static constexpr float FIRImpulseResponse[FilterLength] = {
        0.000019221241265146985,
        -0.00011693312320218828,
        -0.00005145857598133435,
        0.00008581389247283942,
        -0.00012780301900775913,
        0.00010668051341248389,
        -0.000008622727505666933,
        -0.00014028199222028285,
        0.0002685338232764779,
        -0.0002890473752543451,
        0.00014754457642863944,
        0.0001334110847936313,
        -0.00044126984894593514,
        0.0006095914860387138,
        -0.0004971931160782956,
        0.00007621217304807374,
        0.0005184131163431792,
        -0.001016179206714934,
        0.0011211539754743315,
        -0.0006662806500969817,
        -0.0002619526215376982,
        0.0013037060562800817,
        -0.001946857468262416,
        0.001758397683263153,
        -0.0006298476311485767,
        -0.0010894932717156326,
        0.002666628896147836,
        -0.0032733721006291735,
        0.0023868140851515977,
        -0.00012046829336579358,
        -0.002697027834370035,
        0.004787202334285269,
        -0.0049665175529753905,
        0.0027530718998398465,
        0.0012548494659758206,
        -0.005459767344046894,
        0.00785257325051926,
        -0.0069131311342361754,
        0.0024426666581136907,
        0.004087987482699095,
        -0.009930050039884208,
        0.012133969863481234,
        -0.00892158194730952,
        0.0007562479354365467,
        0.00944558165086633,
        -0.017212383024394325,
        0.01830710582698195,
        -0.010748182318473543,
        -0.003809417670638022,
        0.020059787670011972,
        -0.0306426420748565,
        0.02894452747150321,
        -0.01214219453278461,
        -0.016683406564116043,
        0.04825691585523149,
        -0.06879210872427258,
        0.06265533931604236,
        -0.012897348533810412,
        -0.11374662843723274,
        0.6011765454616573,
        0.6011765454616573,
        -0.11374662843723274,
        -0.012897348533810412,
        0.06265533931604236,
        -0.06879210872427258,
        0.04825691585523149,
        -0.016683406564116043,
        -0.01214219453278461,
        0.02894452747150321,
        -0.0306426420748565,
        0.020059787670011972,
        -0.003809417670638022,
        -0.010748182318473543,
        0.01830710582698195,
        -0.017212383024394325,
        0.00944558165086633,
        0.0007562479354365467,
        -0.00892158194730952,
        0.012133969863481234,
        -0.009930050039884208,
        0.004087987482699095,
        0.0024426666581136907,
        -0.0069131311342361754,
        0.00785257325051926,
        -0.005459767344046894,
        0.0012548494659758206,
        0.0027530718998398465,
        -0.0049665175529753905,
        0.004787202334285269,
        -0.002697027834370035,
        -0.00012046829336579358,
        0.0023868140851515977,
        -0.0032733721006291735,
        0.002666628896147836,
        -0.0010894932717156326,
        -0.0006298476311485767,
        0.001758397683263153,
        -0.001946857468262416,
        0.0013037060562800817,
        -0.0002619526215376982,
        -0.0006662806500969817,
        0.0011211539754743315,
        -0.001016179206714934,
        0.0005184131163431792,
        0.00007621217304807374,
        -0.0004971931160782956,
        0.0006095914860387138,
        -0.00044126984894593514,
        0.0001334110847936313,
        0.00014754457642863944,
        -0.0002890473752543451,
        0.0002685338232764779,
        -0.00014028199222028285,
        -0.000008622727505666933,
        0.00010668051341248389,
        -0.00012780301900775913,
        0.00008581389247283942,
        -0.00005145857598133435,
        -0.00011693312320218828,
        0.000019221241265146985
    };

    float Update(float);
};

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <vector>
#include <cstdint>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

#include "../deps/acutest.h" //https://github.com/mity/acutest

#include "../src/memory.h"
#include "../src/bus.h"
#include "../src/6502.h"

#include "../test/6502_ut_arithmetic.h"
#include "../test/6502_ut_branch.h"
#include "../test/6502_ut_inc_dec.h"
#include "../test/6502_ut_jumps_calls.h"
#include "../test/6502_ut_load.h"
#include "../test/6502_ut_logic.h"
#include "../test/6502_ut_reg_tx.h"
#include "../test/6502_ut_shifts.h"
#include "../test/6502_ut_stack_ops.h"
#include "../test/6502_ut_status_flag_change.h"
#include "../test/6502_ut_store.h"
#include "../test/6502_ut_sys_funcs.h"

TEST_LIST = {
    {"BEQCanBranchForwardsIfEqual", BEQCanBranchForwardsIfEqual},
    {"BEQCanBranchForwardsAndCrossPageBoundaryIfEqual", BEQCanBranchForwardsAndCrossPageBoundaryIfEqual},
    {"BEQDoesNotBranchIfNotEqual", BEQDoesNotBranchIfNotEqual},
    {"BEQCanBranchBackwardsIfEqual", BEQCanBranchBackwardsIfEqual},
    {"BNECanBranchForwardsIfNotEqual", BNECanBranchForwardsIfNotEqual},
    {"BNECanBranchForwardsAndCrossPageBoundaryIfNotEqual", BNECanBranchForwardsAndCrossPageBoundaryIfNotEqual},
    {"BNEDoesNotBranchIfEqual", BNEDoesNotBranchIfEqual},
    {"BNECanBranchBackwardsIfNotEqual", BNECanBranchBackwardsIfNotEqual},
    {"BCSCanBranchForwardsIfCarrySet", BCSCanBranchForwardsIfCarrySet},
    {"BCCCanBranchForwardsIfCarryClear", BCCCanBranchForwardsIfCarryClear},
    {"BMICanBranchForwardsIfNegativeSet", BMICanBranchForwardsIfNegativeSet},
    {"BPLCanBranchForwardsIfNegativeClear", BPLCanBranchForwardsIfNegativeClear},
    {"BVSCanBranchForwardsIfOverflowSet", BVSCanBranchForwardsIfOverflowSet},
    {"BVCCanBranchForwardsIfOverflowClear", BVCCanBranchForwardsIfOverflowClear},

    {"INXCanIncrementAZeroValue", INXCanIncrementAZeroValue},
    {"INXCanIncrementToZero", INXCanIncrementToZero},
    {"INXCanIncrementToANegativeValue", INXCanIncrementToANegativeValue},
    {"INYCanIncrementAZeroValue", INYCanIncrementAZeroValue},
    {"INYCanIncrementToZero", INYCanIncrementToZero},
    {"INYCanIncrementToANegativeValue", INYCanIncrementToANegativeValue},
    {"DEXCanDecrementANegativeValue", DEXCanDecrementANegativeValue},
    {"DEXCanDecrementToZero", DEXCanDecrementToZero},
    {"DEXCanDecrementAPositiveValue", DEXCanDecrementAPositiveValue},
    {"INCZPCanIncrementAPositiveValue", INCZPCanIncrementAPositiveValue},
    {"INCZPXCanIncrementAPositiveValue", INCZPXCanIncrementAPositiveValue},
    {"INCAbsCanIncrementAPositiveValue", INCAbsCanIncrementAPositiveValue},
    {"INCAbsXCanIncrementAPositiveValue", INCAbsXCanIncrementAPositiveValue},
    {"DECZPCanDecrementAPositiveValue", DECZPCanDecrementAPositiveValue},
    {"DECZPXCanDecrementAPositiveValue", DECZPXCanDecrementAPositiveValue},
    {"DECAbsCanDecrementAPositiveValue", DECAbsCanDecrementAPositiveValue},
    {"DECAbsXCanDecrementAPositiveValue", DECAbsXCanDecrementAPositiveValue},

    {"CanJumpToASubroutineAndJumpBack",CanJumpToASubroutineAndJumpBack},
    {"JSRDoesNotAffectProcessorStatus",JSRDoesNotAffectProcessorStatus},
    {"RTSDoesNotAffectProcessorStatus",RTSDoesNotAffectProcessorStatus},
    {"JumpAbsCanJumpToANewLocationInProgram",JumpAbsCanJumpToANewLocationInProgram},
    {"JumpIndCanJumpToANewLocationInProgram",JumpIndCanJumpToANewLocationInProgram},

    {"LDAImmCanLoadAValueIntoARegr", LDAImmCanLoadAValueIntoARegr},
    {"LDAImmCanLoadAZeroValueIntoARegr", LDAImmCanLoadAZeroValueIntoARegr},
    {"LDAZPCanLoadAValueIntoARegr", LDAZPCanLoadAValueIntoARegr},
    {"LDAZPXCanLoadAValueIntoARegr", LDAZPXCanLoadAValueIntoARegr},
    {"LDAZPXCanLoadAValueIntoARegrWhenItWraps", LDAZPXCanLoadAValueIntoARegrWhenItWraps},
    {"LDAAbsCanLoadAValueIntoARegr", LDAAbsCanLoadAValueIntoARegr},
    {"LDAAbsXCanLoadAValueIntoARegr", LDAAbsXCanLoadAValueIntoARegr},
    {"LDAAbsXCanLoadAValueIntoARegrWhenItCrossesPageBndry", LDAAbsXCanLoadAValueIntoARegrWhenItCrossesPageBndry},
    {"LDAAbsYCanLoadAValueIntoARegr", LDAAbsYCanLoadAValueIntoARegr},
    {"LDAAbsYCanLoadAValueIntoARegrWhenItCrossesPageBndry", LDAAbsYCanLoadAValueIntoARegrWhenItCrossesPageBndry},
    {"LDAIndXCanLoadAValueIntoARegr", LDAIndXCanLoadAValueIntoARegr},
    {"LDAIndYCanLoadAValueIntoARegr", LDAIndYCanLoadAValueIntoARegr},
    {"LDAIndYCanLoadAValueIntoARegrWhenItCrossesPageBndry", LDAIndYCanLoadAValueIntoARegrWhenItCrossesPageBndry},
    {"LDXImmCanLoadAValueIntoXRegr", LDXImmCanLoadAValueIntoXRegr},
    {"LDYImmCanLoadAValueIntoYRegr", LDYImmCanLoadAValueIntoYRegr},
    {"LDXZPCanLoadAValueIntoXRegr", LDXZPCanLoadAValueIntoXRegr},
    {"LDYZPCanLoadAValueIntoYRegr", LDYZPCanLoadAValueIntoYRegr},
    {"LDXZPYCanLoadAValueIntoXRegr", LDXZPYCanLoadAValueIntoXRegr},
    {"LDYZPXCanLoadAValueIntoYRegr", LDYZPXCanLoadAValueIntoYRegr},
    {"LDYZPXCanLoadAValueIntoYRegrWhenItWraps", LDYZPXCanLoadAValueIntoYRegrWhenItWraps},
    {"LDXAbsCanLoadAValueIntoXRegr", LDXAbsCanLoadAValueIntoXRegr},
    {"LDYAbsCanLoadAValueIntoYRegr", LDYAbsCanLoadAValueIntoYRegr},
    {"LDXAbsYCanLoadAValueIntoXRegrWhenItCrossesPageBndry", LDXAbsYCanLoadAValueIntoXRegrWhenItCrossesPageBndry},
    {"LDXAbsYCanLoadAValueIntoXRegr", LDXAbsYCanLoadAValueIntoXRegr},
    {"LDYAbsXCanLoadAValueIntoYRegr", LDYAbsXCanLoadAValueIntoYRegr},
    {"LDYAbsXCanLoadAValueIntoYRegrWhenItCrossesPageBndry", LDYAbsXCanLoadAValueIntoYRegrWhenItCrossesPageBndry},

    {"ANDImmCanANDAValueFromMemory", ANDImmCanANDAValueFromMemory},
    {"ANDImmCanANDAZeroValueFromMemory", ANDImmCanANDAZeroValueFromMemory},
    {"ANDImmCanANDToResultInANegativeValue", ANDImmCanANDToResultInANegativeValue},
    {"EORImmCanEORAValueFromMemory", EORImmCanEORAValueFromMemory},
    {"EORImmCanEORAZeroValueFromMemory", EORImmCanEORAZeroValueFromMemory},
    {"EORImmCanEORToResultInANegativeValue", EORImmCanEORToResultInANegativeValue},
    {"ORAImmCanORAValueFromMemory", ORAImmCanORAValueFromMemory},
    {"ORAImmCanORAZeroValueFromMemory", ORAImmCanORAZeroValueFromMemory},
    {"ORAImmCanORAToResultInANegativeValue", ORAImmCanORAToResultInANegativeValue},
    {"ANDZPCanANDAValueFromMemory", ANDZPCanANDAValueFromMemory},
    {"EORZPCanEORAValueFromMemory", EORZPCanEORAValueFromMemory},
    {"ORAZPCanORAValueFromMemory", ORAZPCanORAValueFromMemory},
    {"ANDZPXCanANDAValueFromMemory", ANDZPXCanANDAValueFromMemory},
    {"EORZPXCanEORAValueFromMemory", EORZPXCanEORAValueFromMemory},
    {"ORAZPXCanORAValueFromMemory", ORAZPXCanORAValueFromMemory},
    {"ANDAbsCanANDAValueFromMemory", ANDAbsCanANDAValueFromMemory},
    {"EORAbsCanEORAValueFromMemory", EORAbsCanEORAValueFromMemory},
    {"ORAAbsCanORAValueFromMemory", ORAAbsCanORAValueFromMemory},
    {"BITZPCanUpdateStatusFlags01", BITZPCanUpdateStatusFlags01},
    {"BITZPCanUpdateStatusFlags02", BITZPCanUpdateStatusFlags02},
    {"BITAbsCanUpdateStatusFlags01", BITAbsCanUpdateStatusFlags01},
    {"BITAbsCanUpdateStatusFlags02", BITAbsCanUpdateStatusFlags02},

    {"TAXCanTransferPositiveARegToXReg", TAXCanTransferPositiveARegToXReg},
    {"TAXCanTransferZeroARegToXReg", TAXCanTransferZeroARegToXReg},
    {"TAXCanTransferNegativeARegToXReg", TAXCanTransferNegativeARegToXReg},
    {"TAYCanTransferPositiveARegToYReg", TAYCanTransferPositiveARegToYReg},
    {"TAYCanTransferZeroARegToYReg", TAYCanTransferZeroARegToYReg},
    {"TAYCanTransferNegativeARegToYReg", TAYCanTransferNegativeARegToYReg},
    {"TXACanTransferPositiveXRegToAReg", TAYCanTransferPositiveARegToYReg},
    {"TXACanTransferZeroXRegToAReg", TAYCanTransferZeroARegToYReg},
    {"TXACanTransferNegativeXRegToAReg", TAYCanTransferNegativeARegToYReg},
    {"TYACanTransferPositiveYRegToAReg", TAYCanTransferPositiveARegToYReg},
    {"TYACanTransferZeroYRegToAReg", TAYCanTransferZeroARegToYReg},
    {"TYACanTransferNegativeYRegToAReg", TAYCanTransferNegativeARegToYReg},

    {"TSXCanTransferStackPointerToXRegister",TSXCanTransferStackPointerToXRegister},
    {"TSXCanTransferAZeroStackPointerToXRegister",TSXCanTransferAZeroStackPointerToXRegister},
    {"TSXCanTransferANegativeStackPointerToXRegister",TSXCanTransferANegativeStackPointerToXRegister},
    {"TXSCanTransferXRegisterToStackPointer",TXSCanTransferXRegisterToStackPointer},
    {"PHACanPushARegisterOntoTheStack",PHACanPushARegisterOntoTheStack},
    {"PHPCanPushProcessorStatusOntoTheStack",PHPCanPushProcessorStatusOntoTheStack},
    {"PLACanPullAValueFromStackToARegister",PLACanPullAValueFromStackToARegister},
    {"PLACanPullAZeroValueFromStackToARegister",PLACanPullAZeroValueFromStackToARegister},
    {"PLACanPullANegativeValueFromStackToARegister",PLACanPullANegativeValueFromStackToARegister},
    {"PLPCanPullAValueFromStackToProcessorStatus",PLPCanPullAValueFromStackToProcessorStatus},

    {"STAZeroPageCanStoreARegisterIntoMemory",STAZeroPageCanStoreARegisterIntoMemory},
    {"STAZeroPageXCanStoreARegisterIntoMemory",STAZeroPageXCanStoreARegisterIntoMemory},
    {"STAZeroPageXCanStoreARegisterIntoMemoryWhenItWraps",STAZeroPageXCanStoreARegisterIntoMemoryWhenItWraps},
    {"STXZeroPageCanStoreXRegisterIntoMemory",STXZeroPageCanStoreXRegisterIntoMemory},
    {"STXZeroPageYCanStoreXRegisterIntoMemory",STXZeroPageYCanStoreXRegisterIntoMemory},
    {"STXZeroPageYCanStoreXRegisterIntoMemoryWhenItWraps",STXZeroPageYCanStoreXRegisterIntoMemoryWhenItWraps},
    {"STYZeroPageCanStoreYRegisterIntoMemory",STYZeroPageCanStoreYRegisterIntoMemory},
    {"STYZeroPageXCanStoreYRegisterIntoMemory",STYZeroPageXCanStoreYRegisterIntoMemory},
    {"STYZeroPageXCanStoreYRegisterIntoMemoryWhenItWraps",STYZeroPageXCanStoreYRegisterIntoMemoryWhenItWraps},
    {"STAAbsCanStoreARegisterIntoMemory",STAAbsCanStoreARegisterIntoMemory},
    {"STXAbsCanStoreXRegisterIntoMemory",STXAbsCanStoreXRegisterIntoMemory},
    {"STYAbsCanStoreYRegisterIntoMemory",STYAbsCanStoreYRegisterIntoMemory},
    {"STAAbsXCanStoreARegisterIntoMemory",STAAbsXCanStoreARegisterIntoMemory},
    {"STAAbsYCanStoreARegisterIntoMemory",STAAbsYCanStoreARegisterIntoMemory},
    {"STAIndXCanStoreARegisterIntoMemory",STAIndXCanStoreARegisterIntoMemory},
    {"STAIndXCanStoreARegisterIntoMemoryWhenItWraps",STAIndXCanStoreARegisterIntoMemoryWhenItWraps},
    {"STAIndYCanStoreARegisterIntoMemory",STAIndYCanStoreARegisterIntoMemory},
    {"CLCCanClearCarryFlag",CLCCanClearCarryFlag},
    {"CLDCanClearDecimalModeFlag",CLDCanClearDecimalModeFlag},
    {"CLICanClearInterruptDisableFlag",CLICanClearInterruptDisableFlag},
    {"CLVCanClearOverflowFlag",CLVCanClearOverflowFlag},
    {"SECCanSetCarryFlagFlag",SECCanSetCarryFlag},
    {"SEDCanSetDecimalModeFlag",SEDCanSetDecimalModeFlag},
    {"SEICanSetInterruptDisableFlag",SEICanSetInterruptDisableFlag},
    //SYSTEM FUNCTIONS
    {"BRKWillLoadTheProgramCounterFromTheInterruptVector",BRKWillLoadTheProgramCounterFromTheInterruptVector},
    {"BRKWillStoreTheProgramCounterInTheStack",BRKWillStoreTheProgramCounterInTheStack},
    {"BRKWillStoreTheProcessorStatusInTheStack",BRKWillStoreTheProcessorStatusInTheStack},
    {"BRKWillCorrectlyPushThreeBytesOntoTheStack",BRKWillCorrectlyPushThreeBytesOntoTheStack},
    {"BRKWillSetTheInterruptDisableFlag",BRKWillSetTheInterruptDisableFlag},
    {"BRKWillSetTheInterruptDisableFlag",BRKWillSetTheInterruptDisableFlag},
    {"BRKWillSetTheBFlagOnTheProcessorStatusInTheStackOnly",BRKWillSetTheBFlagOnTheProcessorStatusInTheStackOnly},
    {"RTIWillLoadTheStackPointerAndProgramCounterFromTheStack",RTIWillLoadTheStackPointerAndProgramCounterFromTheStack},
    {"NMIWillCorrectlyPushThreeBytesOntoTheStack",NMIWillCorrectlyPushThreeBytesOntoTheStack},

    {"ADCImmWillAddTwoPositiveNumbers",ADCImmWillAddTwoPositiveNumbers},
    {"ADCZPWillAddTwoPositiveNumbers",ADCZPWillAddTwoPositiveNumbers},
    {"ADCZPXWillAddTwoPositiveNumbers",ADCZPXWillAddTwoPositiveNumbers},
    {"ADCAbsWillAddZeroToZero",ADCAbsWillAddZeroToZero},
    {"ADCAbsWillAddZeroToZeroWithCarry",ADCAbsWillAddZeroToZeroWithCarry},
    {"ADCAbsWillAddOneToFFAndSetCarry",ADCAbsWillAddOneToFFAndSetCarry},
    {"ADCAbsWillSetNegativeFlagWhenTheResultIsNegative",ADCAbsWillSetNegativeFlagWhenTheResultIsNegative},
    {"ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithNegativeInputs",ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithNegativeInputs},
    {"ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithPositiveInputs",ADCAbsWillSetOverflowFlagWhenSignedAdditionFailsWithPositiveInputs},
    {"ADCAbsXWillAddTwoPositiveNumbers",ADCAbsXWillAddTwoPositiveNumbers},
    {"ADCAbsYWillAddTwoPositiveNumbers",ADCAbsYWillAddTwoPositiveNumbers},
    {"ADCIndXWillAddTwoPositiveNumbers",ADCIndXWillAddTwoPositiveNumbers},
    {"ADCIndYWillAddTwoPositiveNumbers",ADCIndYWillAddTwoPositiveNumbers},

    {"SBCAbsWillSubtractZeroFromZero",SBCAbsWillSubtractZeroFromZero},
    {"SBCAbsWillSubtractOneFromZeroWithCarry",SBCAbsWillSubtractOneFromZeroWithCarry},
    {"SBCAbsWillSubtractOneFromZero",SBCAbsWillSubtractOneFromZero},
    {"SBCAbsWillSubtractOneFromOne",SBCAbsWillSubtractOneFromOne},
    {"SBCAbsWillSubtractTwoNegativeNumbersAndGetSignedOverflow",SBCAbsWillSubtractTwoNegativeNumbersAndGetSignedOverflow},
    {"SBCAbsWillSubtractAPositiveAndNegativeNumberAndGetSignedOverflow",SBCAbsWillSubtractAPositiveAndNegativeNumberAndGetSignedOverflow},

    {"CMPImmCanCompareLargePosAToSmallPosM",CMPImmCanCompareLargePosAToSmallPosM},
    {"CMPImmCanCompareSmallPosAToLargePosM",CMPImmCanCompareSmallPosAToLargePosM},
    {"CMPZPCanCompareLargePosAToSmallPosM",CMPZPCanCompareLargePosAToSmallPosM},
    {"CMPZPXCanCompareLargePosAToSmallPosM",CMPZPXCanCompareLargePosAToSmallPosM},
    {"CMPAbsCanCompareLargePosAToSmallPosM",CMPAbsCanCompareLargePosAToSmallPosM},
    {"CMPAbsXCanCompareLargePosAToSmallPosM",CMPAbsXCanCompareLargePosAToSmallPosM},
    {"CMPAbsYCanCompareLargePosAToSmallPosM",CMPAbsYCanCompareLargePosAToSmallPosM},
    {"CMPIndXCanCompareLargePosAToSmallPosM",CMPIndXCanCompareLargePosAToSmallPosM},
    {"CMPIndYCanCompareLargePosAToSmallPosM",CMPIndYCanCompareLargePosAToSmallPosM},
    {"CPXImmCanCompareLargePosXToSmallPosM",CPXImmCanCompareLargePosXToSmallPosM},
    {"CPXZPCanCompareLargePosXToSmallPosM",CPXZPCanCompareLargePosXToSmallPosM},
    {"CPXAbsCanCompareLargePosXToSmallPosM",CPXAbsCanCompareLargePosXToSmallPosM},
    {"CPYImmCanCompareLargePosYToSmallPosM",CPYImmCanCompareLargePosYToSmallPosM},
    {"CPYZPCanCompareLargePosYToSmallPosM",CPYZPCanCompareLargePosYToSmallPosM},
    {"CPYAbsCanCompareLargePosYToSmallPosM",CPYAbsCanCompareLargePosYToSmallPosM},

    {"ASLACCCanShiftTheValueOfZero",ASLACCCanShiftTheValueOfZero},
    {"ASLACCCanShiftTheValueOfOne",ASLACCCanShiftTheValueOfOne},
    {"ASLACCCanShiftAPositiveValueToNegative",ASLACCCanShiftAPositiveValueToNegative},
    {"ASLACCCanShiftANegativeValueToCarry",ASLACCCanShiftANegativeValueToCarry},
    {"ASLACCCanShiftANegativeValueToZero",ASLACCCanShiftANegativeValueToZero},
    {"ASLZPCanShiftTheValueOfOne",ASLZPCanShiftTheValueOfOne},
    {"ASLZPXCanShiftTheValueOfOne",ASLZPXCanShiftTheValueOfOne},
    {"ASLABSCanShiftTheValueOfOne",ASLABSCanShiftTheValueOfOne},
    {"ASLABSXCanShiftTheValueOfOne",ASLABSXCanShiftTheValueOfOne},

    {"LSRACCCanShiftTheValueOfZero",LSRACCCanShiftTheValueOfZero},
    {"LSRACCCanShiftTheValueOfOne",LSRACCCanShiftTheValueOfOne},
    {"LSRACCCanShiftANegativeValueToPositive",LSRACCCanShiftANegativeValueToPositive},
    {"LSRZPCanShiftTheValueOfOne",LSRZPCanShiftTheValueOfOne},
    {"LSRZPXCanShiftTheValueOfOne",LSRZPXCanShiftTheValueOfOne},
    {"LSRABSCanShiftTheValueOfOne",LSRABSCanShiftTheValueOfOne},
    {"LSRABSXCanShiftTheValueOfOne",LSRABSXCanShiftTheValueOfOne},

    {"ROLACCCanShiftTheValueOfOneWithoutCarry",ROLACCCanShiftTheValueOfOneWithoutCarry},
    {"ROLACCCanShiftTheValueOfOneWithCarry",ROLACCCanShiftTheValueOfOneWithCarry},
    {"ROLACCCanShiftAPositiveValueToNegative",ROLACCCanShiftAPositiveValueToNegative},
    {"ROLACCCanShiftANegativeValueToCarry",ROLACCCanShiftANegativeValueToCarry},
    {"ROLZPCanShiftTheValueOfOne",ROLZPCanShiftTheValueOfOne},
    {"ROLZPXCanShiftTheValueOfOne",ROLZPXCanShiftTheValueOfOne},
    {"ROLABSCanShiftTheValueOfOne",ROLABSCanShiftTheValueOfOne},
    {"ROLABSXCanShiftTheValueOfOne",ROLABSXCanShiftTheValueOfOne},

    {"RORACCCanShiftTheValueOfOneWithoutCarry",RORACCCanShiftTheValueOfOneWithoutCarry},
    {"RORACCCanShiftTheValueOfOneWithCarry",RORACCCanShiftTheValueOfOneWithCarry},
    {"RORACCCanShiftANegativeValueToPositive",RORACCCanShiftANegativeValueToPositive},
    {"RORZPCanShiftTheValueOfTwo",RORZPCanShiftTheValueOfTwo},
    {"RORZPXCanShiftTheValueOfTwo",RORZPXCanShiftTheValueOfTwo},
    {"RORABSCanShiftTheValueOfTwo",RORABSCanShiftTheValueOfTwo},
    {"RORABSXCanShiftTheValueOfTwo",RORABSXCanShiftTheValueOfTwo},
    {"RORABSXCanShiftTheValueOfTwo",RORABSXCanShiftTheValueOfTwo},

    { NULL, NULL }     /* zeroed record marking the end of the list */
};

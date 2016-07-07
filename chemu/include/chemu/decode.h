#ifndef _DECODE_H
#define _DECODE_H

#include "ChipInstFunc.h"
#include "ChipInst.h"

extern const ChipInstFunc CHIP_OPTABLE[];

ChipInstFunc chipdec_decode(ChipInst instruction);



#endif

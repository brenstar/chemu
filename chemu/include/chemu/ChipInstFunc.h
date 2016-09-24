#ifndef _CHIPINSTFUNC_H
#define _CHIPINSTFUNC_H

#include "ChipEmu.h"
#include "ChipMem.h"
#include "ChipInstDec.h"
#include "ChipInstResult.h"
#include "ChipAddress.h"

// typedef for a function pointer that represents a CHIP-8 instruction
// function. Calling a function of this type is analagous to executing a
// CHIP-8 instruction on the given ChipEmu object.
typedef ChipInstResult (*ChipInstFunc)(ChipEmu, ChipMem*, ChipInstDec);

#endif

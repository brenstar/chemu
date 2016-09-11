#ifndef _DECODE_H
#define _DECODE_H

#include "ChipOp.h"
#include "ChipInstFunc.h"
#include "ChipInst.h"
#include "ChipInstDec.h"

#include "internal/export.h"

// Constant return value for chipdec_index when the given instruction is not
// a valid CHIP-8 instruction
#define NO_INSTRUCTION -1

// Table of ChipOp structures
CHEMU_API extern const ChipOp CHIP_OPTABLE[];

// Macro calculates the address of an instruction, x. The address is the lower
// 12 bits of the instruction.
#define addr(x) (x & 0xFFF)

// Calculates the lower byte of an instruction, x.
#define lowerbyte(x) (x & 0xFF)

// Calculates the fourth nibble of an instruction, x. The fourth nibble is bits
// 12-15 or the upper four bits.
#define nibble4(x) (x >> 12)

// Calculates the third nibble of an instruction, x. The third nibble is bits
// 8-11.
#define nibble3(x) ((x & 0x0F00) >> 8)

// Calculates the second nibble of an instruction, x. The second nibble is bits
// 4-7.
#define nibble2(x) ((x & 0x00F0) >> 4)

// Calculates the first nibble of an instruction, x. The first nibble is bits
// 0-3 or the lower four bits.
#define nibble1(x) (x & 0xF)

// Decodes the given instruction as the given class. The returned value can
// then be used to execute a CHIP-8 instruction function.
CHEMU_API ChipInstDec chipdec_decode(ChipInst instruction, ChipInstClass cls);

// Decodes the given instruction and returns the index in the CHIP_OPTABLE.
// Returns NO_INSTRUCTION if the instruction is illegal.
CHEMU_API int chipdec_index(ChipInst instruction);

#endif

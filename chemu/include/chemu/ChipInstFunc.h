#ifndef _CHIPINSTFUNC_H
#define _CHIPINSTFUNC_H

#include "ChipEmu.h"
#include "ChipInstDec.h"
#include "ChipAddress.h"

/* typedef for a function pointer that represents a CHIP-8 instruction
 * function.
 */
typedef ChipInstResult (*ChipInstFunc)(ChipEmu*, ChipInstDec);

typedef enum ChipInstClass_e {
	INST_CLASS_A,			// address class
	INST_CLASS_I,			// immediate class
	INST_CLASS_R,			// register class
	INST_CLASS_V			// void class
} ChipInstClass;

#endif

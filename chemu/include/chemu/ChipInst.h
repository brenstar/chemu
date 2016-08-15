#ifndef _CHIPINST_H
#define _CHIPINST_H

#include <stdint.h>

#include "ChipEmu.h"

typedef enum ChipInstResult_e {
	INST_SUCCESS,			// successful execution
	INST_SUCCESS_INCR_PC,	// successful execution, increment program counter
	INST_FAILURE			// failed exectuion
} ChipInstResult;

typedef uint16_t ChipInst;

#endif

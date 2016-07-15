// V-Type instructions

#include "chemu/instructions.h"

#include "chemu/display.h"
#include "chemu/stack.h"

#define RESERVED emu->memory.reserved

// cls - Clear screen
ChipInstResult cif_cls(ChipEmu *emu, ChipInstDec instruction) {

	chipdisplay_clear(&RESERVED.display);

	return INST_SUCCESS_INCR_PC;
}

// ret - Return from subroutine
ChipInstResult cif_ret(ChipEmu *emu, ChipInstDec instruction) {

	ChipInstResult result;
	if (chipstack_can_pop(&RESERVED.stack)) {
		RESERVED.pc = chipstack_pop(&RESERVED.stack);
		result = INST_SUCCESS_INCR_PC;
	} else {
		result = INST_FAILURE;
	}

	return result;
}

// V-Type instructions

#include "chemu/instructions.h"

#include "chemu/display.h"
#include "chemu/stack.h"
#include "chemu/emulation.h"

#include "chemu/logger.h"

#define RESERVED emu->memory.reserved

// cls - Clear screen
ChipInstResult cif_cls(ChipEmu *emu, ChipInstDec instruction) {
    (void)instruction;

    chipdisplay_clear(&RESERVED.display);
    //emu->flags |= CHIP_REDRAW_FLAG;
    chipemu_redraw(emu);

    return INST_SUCCESS_INCR_PC;
}

// ret - Return from subroutine
ChipInstResult cif_ret(ChipEmu *emu, ChipInstDec instruction) {
    (void)instruction;

    ChipInstResult result;
    if (chipstack_can_pop(&RESERVED.stack)) {
        RESERVED.pc = chipstack_pop(&RESERVED.stack);
        chiplog_debug("[Stack] Popped 0x%03X from call stack\n", RESERVED.pc);
        result = INST_SUCCESS_INCR_PC;
    } else {
        chiplog_error("[Stack] Cannot return, stack is empty\n");
        result = INST_FAILURE;
    }

    return result;
}

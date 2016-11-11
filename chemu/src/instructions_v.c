// V-Type instructions

#include "chemu/instructions.h"

#include "chemu/display.h"
#include "chemu/stack.h"
#include "chemu/emulation.h"

#include "chemu/logger.h"

// cls - Clear screen
ChipInstResult chemu_if_cls(ChipEmu emu, ChipMem *mem, ChipInstDec instruction) {
    (void)instruction;

    chemu_disp_clear(&mem->reserved.display);
    //emu->flags |= CHIP_REDRAW_FLAG;
    chemu_emu_redraw(emu);

    return INST_SUCCESS_INCR_PC;
}

// ret - Return from subroutine
ChipInstResult chemu_if_ret(ChipEmu emu, ChipMem *mem, ChipInstDec instruction) {
    (void)emu; (void)instruction;

    ChipInstResult result;
    if (chemu_stack_canPop(&mem->reserved.stack)) {
        mem->reserved.pc = chemu_stack_pop(&mem->reserved.stack);
        chiplog_debug("[Stack] Popped 0x%03X from call stack\n", mem->reserved.pc);
        result = INST_SUCCESS_INCR_PC;
    } else {
        chiplog_error("[Stack] Cannot return, stack is empty\n");
        result = INST_FAILURE;
    }

    return result;
}

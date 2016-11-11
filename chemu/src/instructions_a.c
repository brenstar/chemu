// A-Type instructions

#include "chemu/instructions.h"
#include "chemu/stack.h"

#include "chemu/logger.h"

// sys - syscall
ChipInstResult chemu_if_sys(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu; (void)mem; (void)inst;
    chiplog_warn("Attempted system call: %d\n", inst.a.addr);
    // not implemented do nothing

    return INST_SUCCESS_INCR_PC;
}

// j - jump to address
ChipInstResult chemu_if_j(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.pc = inst.a.addr;

    return INST_SUCCESS;
}

// call - call subroutine
ChipInstResult chemu_if_call(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    ChipInstResult result;
    if (chemu_stack_canPush(&mem->reserved.stack)) {
        chiplog_debug("[Stack] Pushing 0x%03X onto call stack\n", mem->reserved.pc);
        chemu_stack_push(&mem->reserved.stack, mem->reserved.pc);
        mem->reserved.pc = inst.a.addr;
        result = INST_SUCCESS;
    } else {
        chiplog_error("Failed to call subroutine: stack at limit\n");
        result = INST_FAILURE;
    }

    return result;
}

// la - load address
ChipInstResult chemu_if_la(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.addrReg = inst.a.addr;

    return INST_SUCCESS_INCR_PC;
}

// jo - jump with offset
ChipInstResult chemu_if_jo(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.pc = inst.a.addr + mem->reserved.regs[0];

    return INST_SUCCESS;
}

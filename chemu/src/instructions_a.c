// A-Type instructions

#include "chemu/instructions.h"
#include "chemu/stack.h"


#define RESERVED emu->memory.reserved


// sys - syscall
ChipInstResult cif_sys(ChipEmu *emu, ChipInstDec instruction) {
    //setup(instruction);
    // not implemented do nothing

    return INST_SUCCESS_INCR_PC;
}

// j - jump to address
ChipInstResult cif_j(ChipEmu *emu, ChipInstDec inst) {

    RESERVED.pc = inst.a.addr;

    return INST_SUCCESS;
}

// call - call subroutine
ChipInstResult cif_call(ChipEmu *emu, ChipInstDec inst) {

    ChipInstResult result;
    if (chipstack_can_push(&RESERVED.stack)) {
        chipstack_push(&RESERVED.stack, RESERVED.pc);
        RESERVED.pc = inst.a.addr;
        result = INST_SUCCESS;
    } else {
        result = INST_FAILURE;
    }

    return result;
}

// la - load address
ChipInstResult cif_la(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.addrReg = inst.a.addr;

	return INST_SUCCESS_INCR_PC;
}

// jo - jump with offset
ChipInstResult cif_jo(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.pc = inst.a.addr + RESERVED.regs[0];

	return INST_SUCCESS;
}

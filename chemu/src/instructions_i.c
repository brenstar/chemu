// I-Type instructions

#include "chemu/instructions.h"
#include "chemu/input.h"
#include "chemu/memory.h"
#include "chemu/emulation.h"
#include "chemu/timer.h"

#include <stdlib.h>

// sei - Skip next if equal to immediate
ChipInstResult cif_sei(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    if (mem->reserved.regs[inst.i.rnum] == inst.i.immediate)
        mem->reserved.pc += 2; // skip next instruction

	return INST_SUCCESS_INCR_PC;
}

// sni - Skip next if not equal to immediate
ChipInstResult cif_sni(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    if (mem->reserved.regs[inst.i.rnum] != inst.i.immediate)
        mem->reserved.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// li - load immediate
ChipInstResult cif_li(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.i.rnum] = inst.i.immediate;

    return INST_SUCCESS_INCR_PC;
}

// addi - add immediate
ChipInstResult cif_addi(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.i.rnum] += inst.i.immediate;
    // check for carry ?

    return INST_SUCCESS_INCR_PC;
}

// rnd - random
ChipInstResult cif_rnd(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    // TODO: make a separate module for RNG
    mem->reserved.regs[inst.i.rnum] = (uint8_t)(rand() % 255) & inst.i.immediate;

    return INST_SUCCESS_INCR_PC;
}

// constant immediate instructions --------------------------------------------

#define sipHelper(mem, inst, state) \
    do { \
        ChipReg r = mem->reserved.regs[inst.i.rnum]; \
        if (r < 16) \
            if (chipin_keystate(&mem->reserved.input, (ChipKey)r) == state) \
                mem->reserved.pc += 2; \
    } while (0)

// sip - skip next instruction if pressed
ChipInstResult cif_sip(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    sipHelper(mem, inst, CHIP_KEYSTATE_PRESSED);

    return INST_SUCCESS_INCR_PC;
}

// snip - skip next instruction if not pressed
ChipInstResult cif_snip(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    sipHelper(mem, inst, CHIP_KEYSTATE_RELEASED);

    return INST_SUCCESS_INCR_PC;
}

// ld - load delay timer
ChipInstResult cif_ld(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.i.rnum] = mem->reserved.delTimer;

    return INST_SUCCESS_INCR_PC;
}

// lk - wait and load key press
ChipInstResult cif_lk(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    // Note that this instruction will block until chipemu_setKey is called
    mem->reserved.regs[inst.i.rnum] = chipemu_getKey(emu);

    return INST_SUCCESS_INCR_PC;
}

// del - set delay timer
ChipInstResult cif_del(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.delTimer = mem->reserved.regs[inst.i.rnum];

    return INST_SUCCESS_INCR_PC;
}

// snd - set sound timer
ChipInstResult cif_snd(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.sndTimer = mem->reserved.regs[inst.i.rnum];

    return INST_SUCCESS_INCR_PC;
}

// ii - increment address register
ChipInstResult cif_ii(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.addrReg += mem->reserved.regs[inst.i.rnum];

    return INST_SUCCESS_INCR_PC;
}

// font - set address register to font sprite
ChipInstResult cif_font(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    uint8_t vx = mem->reserved.regs[inst.i.rnum];
    if (vx < 16)
        mem->reserved.addrReg = chipmem_get_font(vx);

    return INST_SUCCESS_INCR_PC;
}

// bcd - binary coded decimal
ChipInstResult cif_bcd(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    ChipReg regX = mem->reserved.regs[inst.i.rnum];
    ChipAddress addr = mem->reserved.addrReg + 2;

    for (int i = 0; i < 3; ++i) {
        //emu->memory.array[addr - i] = regX % 10;
        chipmem_write(mem, addr - i, regX % 10);
        regX /= 10;
    }

    return INST_SUCCESS_INCR_PC;
}

// save - save registers in memory
ChipInstResult cif_save(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    ChipAddress addr = mem->reserved.addrReg;
    for (int i = 0; i <= inst.i.rnum; ++i) {
        //emu->memory.array[addr] = emu->dp.regs[i];
        chipmem_write(mem, addr, mem->reserved.regs[i]);
        if (++addr > CHIP_END)
            return INST_FAILURE; // address register is out of bounds
    }

    return INST_SUCCESS_INCR_PC;
}

// rest - restore registers from memory
ChipInstResult cif_rest(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    ChipAddress addr = mem->reserved.addrReg;
    for (int i = 0; i <= inst.i.rnum; ++i) {
        mem->reserved.regs[i] = chipmem_read(mem, addr);
        if (++addr > CHIP_END)
            return INST_FAILURE; // address register is out of bounds
    }

    return INST_SUCCESS_INCR_PC;
}

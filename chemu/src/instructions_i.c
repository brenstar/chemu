// I-Type instructions

#include "chemu/instructions.h"
#include "chemu/input.h"
#include "chemu/memory.h"

#include <stdlib.h>

// shortcut for accessing the reserved memory in variable emu
#define RESERVED emu->memory.reserved

static bool sip_helper(ChipEmu *emu, ChipInstDec inst, ChipKeyState state);

// sei - Skip next if equal to immediate
ChipInstResult cif_sei(ChipEmu *emu, ChipInstDec inst) {

	if (RESERVED.regs[inst.i.rnum] == inst.i.immediate)
		RESERVED.pc += 2; // skip next instruction

	return INST_SUCCESS_INCR_PC;
}

// sni - Skip next if not equal to immediate
ChipInstResult cif_sni(ChipEmu *emu, ChipInstDec inst) {

	if (RESERVED.regs[inst.i.rnum] != inst.i.immediate)
	   RESERVED.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// li - load immediate
ChipInstResult cif_li(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.i.rnum] = inst.i.immediate;

	return INST_SUCCESS_INCR_PC;
}

// addi - add immediate
ChipInstResult cif_addi(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.i.rnum] += inst.i.immediate;

	return INST_SUCCESS_INCR_PC;
}

// rnd - random
ChipInstResult cif_rnd(ChipEmu *emu, ChipInstDec inst) {
    // TODO: make a separate module for RNG
	RESERVED.regs[inst.i.rnum] = (uint8_t)(rand() % 255) & inst.i.immediate;

	return INST_SUCCESS_INCR_PC;
}

// constant immediate instructions --------------------------------------------

// sip - skip next instruction if pressed
ChipInstResult cif_sip(ChipEmu *emu, ChipInstDec inst) {
	if (sip_helper(emu, inst, CHIP_KEYSTATE_PRESSED))
		RESERVED.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// snip - skip next instruction if not pressed
ChipInstResult cif_snip(ChipEmu *emu, ChipInstDec inst) {
	if (sip_helper(emu, inst, CHIP_KEYSTATE_RELEASED))
		RESERVED.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// ld - load delay timer
ChipInstResult cif_ld(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.i.rnum] = RESERVED.delTimer;

	return INST_SUCCESS_INCR_PC;
}

// lk - wait and load key press
ChipInstResult cif_lk(ChipEmu *emu, ChipInstDec inst) {
	RESERVED.regs[inst.i.rnum] = emu->pollKeyHandler();

	return INST_SUCCESS_INCR_PC;
}

// del - set delay timer
ChipInstResult cif_del(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.delTimer = RESERVED.regs[inst.i.rnum];

	return INST_SUCCESS_INCR_PC;
}

// snd - set sound timer
ChipInstResult cif_snd(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.sndTimer = RESERVED.regs[inst.i.rnum];

	return INST_SUCCESS_INCR_PC;
}

// ii - increment address register
ChipInstResult cif_ii(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.addrReg += RESERVED.regs[inst.i.rnum];

	return INST_SUCCESS_INCR_PC;
}

// font - set address register to font sprite
ChipInstResult cif_font(ChipEmu *emu, ChipInstDec inst) {

	uint8_t vx = RESERVED.regs[inst.i.rnum];
	if (vx < 16)
		RESERVED.addrReg = chipmem_get_font(&emu->memory, vx);

	return INST_SUCCESS_INCR_PC;
}

// bcd - binary coded decimal
ChipInstResult cif_bcd(ChipEmu *emu, ChipInstDec inst) {

	ChipReg regX = RESERVED.regs[inst.i.rnum];
	ChipAddress addr = RESERVED.addrReg + 2;

	for (int i = 0; i < 3; ++i) {
		//emu->memory.array[addr - i] = regX % 10;
        chipmem_write(&emu->memory, addr - i, regX % 10);
		regX /= 10;
	}

	return INST_SUCCESS_INCR_PC;
}

// save - save registers in memory
ChipInstResult cif_save(ChipEmu *emu, ChipInstDec inst) {

	ChipAddress addr = RESERVED.addrReg;
	for (int i = 0; i <= inst.i.rnum; ++i) {
		//emu->memory.array[addr] = emu->dp.regs[i];
        chipmem_write(&emu->memory, addr, RESERVED.regs[i]);
        if (++addr > CHIP_END)
			return INST_FAILURE; // address register is out of bounds
	}

	return INST_SUCCESS_INCR_PC;
}

// rest - restore registers from memory
ChipInstResult cif_rest(ChipEmu *emu, ChipInstDec inst) {

	ChipAddress addr = RESERVED.addrReg;
	for (int i = 0; i <= inst.i.rnum; ++i) {
		RESERVED.regs[i] = chipmem_read(&emu->memory, addr);
		if (++addr > CHIP_END)
			return INST_FAILURE; // address register is out of bounds
	}

	return INST_SUCCESS_INCR_PC;
}

// ----------------------------------------------------------------------------
// Private helper functions

// Determines if the key stored in rnum of inst has the given state
// Note: if the value in register rnum is >= 16, false is returned
inline
static bool sip_helper(ChipEmu *emu, ChipInstDec inst, ChipKeyState state) {
	ChipReg regVal = RESERVED.regs[inst.i.rnum];
	return regVal < 16 && chipin_keystate(&RESERVED.input, (ChipKey)regVal) == state;
}

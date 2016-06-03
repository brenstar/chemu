
#include <assert.h>
#include <stdlib.h>

#include "instructions.h"
#include "stack.h"
#include "input.h"

#include "debug.h"

static bool sip_helper(ChipEmu *emu, ChipInst_IType inst, ChipKeyState state);

// ============================================================================
// A-Type (address)
// ============================================================================

// sys - system call
int cif_sys(ChipEmu *emu, ChipInst instruction) {
	ChipInst_AType inst = instruction.atype;
	assert(inst.reserved == 0);
	(void)emu;

	// not implemented, log warning
	logWarn("Attempted system call at 0x%03x\n", inst.addr);

	return INST_SUCCESS_INCR_PC;
}

// j - Jump to address
int cif_j(ChipEmu *emu, ChipInst instruction) {
	ChipInst_AType inst = instruction.atype;
	assert(inst.reserved == 1);

	emu->dp.pc = inst.addr;

	return INST_SUCCESS;
}

// call - Call subroutine
int cif_call(ChipEmu *emu, ChipInst instruction) {
	ChipInst_AType inst = instruction.atype;
	assert(inst.reserved == 2);

	// add pc to stack

	int result;
	if (chipstack_can_push(&emu->stack)) {
		chipstack_push(&emu->stack, emu->dp.pc);
		emu->dp.pc = inst.addr;
		result = INST_SUCCESS;
	} else {
		logError("Failed to call: Call stack at maximum\n");
		result = INST_FAILURE;
	}

	return result;
}

// la - load address
int cif_la(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.atype.reserved == 0xA);

	emu->dp.addrReg = instruction.atype.addr;

	return INST_SUCCESS_INCR_PC;
}

// jo - jump with offset
int cif_jo(ChipEmu *emu, ChipInst instruction) {
	ChipInst_AType inst = instruction.atype;
	assert(inst.reserved == 0xB);

	emu->dp.pc = inst.addr + emu->dp.regs[0];

	return INST_SUCCESS;
}

// ============================================================================
// D-Type (draw)
// ============================================================================

// draw - draw sprite
int cif_draw(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.dtype.reserved == 0xD);

	// TODO: Draw function

	return INST_SUCCESS_INCR_PC;
}

// ============================================================================
// I-Type (immediate)
// ============================================================================

// sei - Skip next if equal to immediate
int cif_sei(ChipEmu *emu, ChipInst inst) {
	assert(inst.itype.reserved == 0x3);

	if (emu->dp.regs[inst.itype.rnum] == inst.itype.immediate)
		emu->dp.pc += 2; // skip next instruction

	return INST_SUCCESS_INCR_PC;
}

// sni - Skip next if not equal to immediate
int cif_sni(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.itype.reserved == 0x4);

	if (emu->dp.regs[instruction.itype.rnum] == instruction.itype.immediate)
		emu->dp.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// li - load immediate
int cif_li(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.itype.reserved == 0x6);

	emu->dp.regs[instruction.itype.rnum] = instruction.itype.immediate;

	return INST_SUCCESS_INCR_PC;
}

// addi - add immediate
int cif_addi(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.itype.reserved == 0x7);

	emu->dp.regs[instruction.itype.rnum] += instruction.itype.immediate;

	return INST_SUCCESS_INCR_PC;
}

// rnd - random
int cif_rnd(ChipEmu *emu, ChipInst inst) {
	assert(inst.itype.reserved == 0xC);

	emu->dp.regs[inst.itype.rnum] = (uint8_t)(rand() % 255) & inst.itype.immediate;

	return INST_SUCCESS_INCR_PC;
}

// constant immediate instructions --------------------------------------------

// sip - skip next instruction if pressed
int cif_sip(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xE && inst.immediate == 0x9E);

	//uint8_t regVal = emu->dp.regs[inst.rnum];
	//if (regVal < 16 && chipin_keystate(&emu->input, (ChipKey)regVal) == CHIP_KEYSTATE_PRESSED)
	if (sip_helper(emu, inst, CHIP_KEYSTATE_PRESSED))
		emu->dp.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// snip - skip next instruction if not pressed
int cif_snip(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xE && inst.immediate == 0xA1);

	if (sip_helper(emu, inst, CHIP_KEYSTATE_RELEASED))
		emu->dp.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// ld - load delay timer
int cif_ld(ChipEmu *emu, ChipInst inst) {
	assert(inst.itype.reserved == 0xF && inst.itype.immediate == 0x07);

	emu->dp.regs[inst.itype.rnum] = emu->dp.delTimer;

	return INST_SUCCESS_INCR_PC;
}

// lk - wait and load key press
int cif_lk(ChipEmu *emu, ChipInst instruction) {
	return 0;
}

// del - set delay timer
int cif_del(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x15);

	emu->dp.delTimer = emu->dp.regs[inst.rnum];

	return INST_SUCCESS_INCR_PC;
}

// snd - set sound timer
int cif_snd(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x18);

	emu->dp.sndTimer = emu->dp.regs[inst.rnum];

	return INST_SUCCESS_INCR_PC;
}

// ii - increment address register
int cif_ii(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x1E);

	emu->dp.addrReg += emu->dp.regs[inst.rnum];

	return INST_SUCCESS_INCR_PC;
}

// font - set address register to font sprite
int cif_font(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x29);

	uint8_t vx = emu->dp.regs[inst.rnum];
	if (vx < 16)
		emu->dp.addrReg = CHIPMEM_FONTSET_START + (vx * 5);

	return INST_SUCCESS_INCR_PC;
}

// bcd - binary coded decimal
int cif_bcd(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x33);

	uint8_t regX = emu->dp.regs[inst.rnum];
	uint16_t addr = emu->dp.addrReg + 2;

	for (int i = 0; i < 3; ++i) {
		emu->memory.array[addr - i] = regX % 10;
		regX /= 10;
	}

	return INST_SUCCESS_INCR_PC;
}

// save - save registers in memory
int cif_save(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x55);

	uint16_t addr = emu->dp.addrReg;
	for (int i = 0; i <= inst.rnum; ++i) {
		emu->memory.array[addr] = emu->dp.regs[i];
		if (++addr > CHIP_END)
			return INST_FAILURE; // address register is out of bounds
	}

	return INST_SUCCESS_INCR_PC;
}

// rest - restore registers from memory
int cif_rest(ChipEmu *emu, ChipInst instruction) {
	ChipInst_IType inst = instruction.itype;
	assert(inst.reserved == 0xF && inst.immediate == 0x65);

	uint16_t addr = emu->dp.addrReg;
	for (int i = 0; i <= inst.rnum; ++i) {
		emu->dp.regs[i] = emu->memory.array[addr];
		if (++addr > CHIP_END)
			return INST_FAILURE; // address register is out of bounds
	}

	return INST_SUCCESS_INCR_PC;
}

// ============================================================================
// R-Type (register)
// ============================================================================

// se - Skip next if equal
int cif_se(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x5 && inst.rtype.reserved_lo == 0x0);

	if (emu->dp.regs[inst.rtype.rs_num] == emu->dp.regs[inst.rtype.ra_num])
		emu->dp.pc += 2;

	return INST_SUCCESS_INCR_PC;
}



// move - sets register value to another register
int cif_move(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x0);

	emu->dp.regs[inst.rtype.rs_num] = emu->dp.regs[inst.rtype.ra_num];

	return INST_SUCCESS_INCR_PC;
}

// or - bitwise or
int cif_or(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x1);

	emu->dp.regs[inst.rtype.rs_num] |= emu->dp.regs[inst.rtype.ra_num];

	return INST_SUCCESS_INCR_PC;
}

// and - bitwise and
int cif_and(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x2);

	emu->dp.regs[inst.rtype.rs_num] &= emu->dp.regs[inst.rtype.ra_num];

	return INST_SUCCESS_INCR_PC;
}

// xor - bitwise xor
int cif_xor(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x3);

	emu->dp.regs[inst.rtype.rs_num] ^= emu->dp.regs[inst.rtype.ra_num];

	return INST_SUCCESS_INCR_PC;
}

// add - addition
int cif_add(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x4);

	uint8_t vx = inst.rtype.rs_num, vy = inst.rtype.ra_num;
	uint8_t temp = emu->dp.regs[vx];
	emu->dp.regs[vx] += emu->dp.regs[vy];

	// carry detection
	emu->dp.regs[15] = (temp > emu->dp.regs[vx]) ? 1 : 0;

	return INST_SUCCESS_INCR_PC;
}

// sub - subtraction
int cif_sub(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x5);

	uint8_t vx = inst.rtype.rs_num, vy = inst.rtype.ra_num;
	uint8_t temp = emu->dp.regs[vx];
	emu->dp.regs[vx] -= emu->dp.regs[vy];

	// borrow detection
	emu->dp.regs[15] = (temp < emu->dp.regs[vx] ? 1 : 0);

	return INST_SUCCESS_INCR_PC;
}

// shr - shift right by 1
int cif_shr(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x6);

	emu->dp.regs[15] = emu->dp.regs[inst.rtype.rs_num] & 1;
	emu->dp.regs[inst.rtype.rs_num] >>= 1;

	return INST_SUCCESS_INCR_PC;
}

// subn - subtraction, operands swapped
int cif_subn(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0x7);

	// xor swap, swap rs_num and ra_num of inst
	inst.rtype.rs_num ^= inst.rtype.ra_num;
	inst.rtype.ra_num ^= inst.rtype.rs_num;
	inst.rtype.rs_num ^= inst.rtype.ra_num;
	inst.rtype.reserved_lo = 5;

	return cif_sub(emu, inst);
}

// shift left by 1
int cif_shl(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x8 && inst.rtype.reserved_lo == 0xE);

	emu->dp.regs[15] = emu->dp.regs[inst.rtype.rs_num] >> 7;
	emu->dp.regs[inst.rtype.rs_num] <<= 1;

	return INST_SUCCESS_INCR_PC;
}

// sn - skip if not equal
int cif_sn(ChipEmu *emu, ChipInst inst) {
	assert(inst.rtype.reserved_hi == 0x9 && inst.rtype.reserved_lo == 0x0);

	if (emu->dp.regs[inst.rtype.rs_num] != emu->dp.regs[inst.rtype.ra_num])
		emu->dp.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// ============================================================================
// V-Type (void)
// ============================================================================

// cls - Clear screen
int cif_cls(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.instruction == 0x00E0);

	// TODO: clear screen

	//emu->pc += 2;
	return INST_SUCCESS_INCR_PC;
}

// ret - Return from subroutine
int cif_ret(ChipEmu *emu, ChipInst instruction) {
	assert(instruction.instruction == 0x00EE);

	int result;
	if (chipstack_can_pop(&emu->stack)) {
		emu->dp.pc = chipstack_pop(&emu->stack);
		result = INST_SUCCESS_INCR_PC;
	} else {
		result = INST_FAILURE;
	}

	/*
	if (emu->sp != emu->stack && emu->sp != NULL) {
		emu->pc = *(emu->sp--); // pc gets the instruction pointed by sp
		result = INST_SUCCESS;
	} else {

		result = INST_FAILURE;
	}*/

	return result;
}

// ----------------------------------------------------------------------------
// Private helper functions

// Determines if the key stored in rnum of inst has the given state
// Note: if the value in register rnum is >= 16, false is returned
static bool sip_helper(ChipEmu *emu, ChipInst_IType inst, ChipKeyState state) {
	uint8_t regVal = emu->dp.regs[inst.rnum];
	return regVal < 16 && chipin_keystate(&emu->input, (ChipKey)regVal) == state;
}

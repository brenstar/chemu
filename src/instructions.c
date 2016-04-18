
#include <assert.h>
#include <stdlib.h>
#include "instructions.h"
#include "chipstack.h"

#define addr(x) (x & 0x0FFF)
#define lowerbyte(x) (x & 0x00FF)
#define nibble3(x) ((x & 0x0F00) >> 8)
#define nibble2(x) ((x & 0x00F0) >> 4)
#define nibble1(x) (x & 0x000F)


// sys - system call
int cif_sys(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0);
	(void)emu;

	// not implemented, do nothing

	return INST_SUCCESS_INCR_PC;
}

// cls - Clear screen
int cif_cls(ChipEmu emu, uint16_t instruction) {
	assert(instruction == 0x00E0);

	// TODO: clear screen

	//emu->pc += 2;
	return INST_SUCCESS_INCR_PC;
}

// ret - Return from subroutine
int cif_ret(ChipEmu emu, uint16_t instruction) {
	assert(instruction == 0x00EE);

	int result;
	if (chipstack_can_pop(emu->stack)) {
		emu->pc = chipstack_pop(emu->stack);
		result = INST_SUCCESS;
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

// j - Jump to address
int cif_j(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x1000);

	emu->pc = addr(instruction);

	return INST_SUCCESS;
}

// call - Call subroutine
int cif_call(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x2000);

	// add pc to stack

	int result;
	if (chipstack_can_push(emu->stack)) {
		chipstack_push(emu->stack, emu->pc);
		cif_j(emu, instruction);
		result = INST_SUCCESS;
	} else {
		result = INST_FAILURE;
	}

	return result;
}

// sei - Skip next if equal to immediate
int cif_sei(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x3000);

	emu->pc += (emu->registers[nibble3(instruction)] == lowerbyte(instruction)) ? 4 : 2;

	return INST_SUCCESS;
}

// sni - Skip next if not equal to immediate
int cif_sni(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x4000);

	emu->pc += (emu->registers[nibble3(instruction)] != lowerbyte(instruction)) ? 4 : 2;

	return INST_SUCCESS;
}

// se - Skip next if equal
int cif_se(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x5000);

	emu->pc += (emu->registers[nibble3(instruction)] ==
	            emu->registers[nibble2(instruction)]) ? 4 : 2;

	return INST_SUCCESS;
}

// li - load immediate
int cif_li(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x6000);

	emu->registers[nibble3(instruction)] = lowerbyte(instruction);

	return INST_SUCCESS_INCR_PC;
}

// addi - add immediate
int cif_addi(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0x7000);

	emu->registers[(instruction & 0x0F00) >> 8] += instruction & 0x00FF;

	return INST_SUCCESS_INCR_PC;
}

// move - sets register value to another register
int cif_move(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8000);

	emu->registers[nibble3(instruction)] = emu->registers[nibble2(instruction)];

	return INST_SUCCESS_INCR_PC;
}

// or - bitwise or
int cif_or(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8001);

	emu->registers[nibble3(instruction)] |= emu->registers[nibble2(instruction)];

	return INST_SUCCESS_INCR_PC;
}

// and - bitwise and
int cif_and(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8002);

	emu->registers[nibble3(instruction)] &= emu->registers[nibble2(instruction)];

	return INST_SUCCESS_INCR_PC;
}

// xor - bitwise xor
int cif_xor(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8003);

	emu->registers[nibble3(instruction)] ^= emu->registers[nibble2(instruction)];

	return INST_SUCCESS_INCR_PC;
}

// add - addition
int cif_add(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8004);

	uint8_t vx = nibble3(instruction), vy = nibble2(instruction);
	uint8_t temp = emu->registers[vx];
	emu->registers[vx] += emu->registers[vy];

	// carry detection
	emu->registers[CARRY_REG] = (temp > emu->registers[vx]) ? 1 : 0;

	return INST_SUCCESS_INCR_PC;
}

// sub - subtraction
int cif_sub(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8005);

	uint8_t vx = nibble3(instruction), vy = nibble2(instruction);
	uint8_t temp = emu->registers[vx];
	emu->registers[vx] -= emu->registers[vy];

	// borrow detection
	emu->registers[CARRY_REG] = (temp < emu->registers[vx] ? 1 : 0);

	return INST_SUCCESS_INCR_PC;
}

// shr - shift right by 1
int cif_shr(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8006);

	emu->registers[CARRY_REG] = emu->registers[nibble3(instruction)] & 1;
	emu->registers[nibble3(instruction)] >>= 1;

	return INST_SUCCESS_INCR_PC;
}

// subn - subtraction, operands swapped
int cif_subn(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x8007);

	uint16_t a = (instruction >> 4) & 0x00F0; // nibble3 in nibble2 position
	uint16_t b = (instruction << 4) & 0x0F00; // nibble2 in nibble3 position

	return cif_sub(emu, (instruction & 0xF00F) | a | b);
}

// shift left by 1
int cif_shl(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x800E);

	emu->registers[CARRY_REG] = emu->registers[nibble3(instruction)] >> 7;
	emu->registers[nibble3(instruction)] <<= 1;

	return INST_SUCCESS_INCR_PC;
}

// sn - skip if not equal
int cif_sn(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF00F) == 0x9000);

	emu->pc += (emu->registers[nibble3(instruction)] !=
	            emu->registers[nibble2(instruction)]) ? 4 : 2;

	return INST_SUCCESS;
}

// la - load address
int cif_la(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0xA000);

	emu->addressReg = addr(instruction);

	return INST_SUCCESS_INCR_PC;
}

// jo - jump with offset
int cif_jo(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0xB000);

	emu->pc = addr(instruction) + emu->registers[0];

	return INST_SUCCESS;
}

// rnd - random
int cif_rnd(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0xC000);

	emu->registers[nibble3(instruction)] = (uint8_t)(rand() % 255) & lowerbyte(instruction);

	return INST_SUCCESS_INCR_PC;
}

// draw - draw sprite
int cif_draw(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF000) == 0xD000);

	// TODO: Draw function

	return INST_SUCCESS_INCR_PC;
}

int cif_sip(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF0FF) == 0xE09E);

	emu->pc += (emu->keymap >> emu->registers[nibble3(instruction)] & 1) ? 4 : 2;

	return INST_SUCCESS;
}

int cif_snip(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF0FF) == 0xE0A1);

	emu->pc += (!(emu->keymap >> emu->registers[nibble3(instruction)] & 1)) ? 4 : 2;

	return INST_SUCCESS;
}

int cif_ld(ChipEmu emu, uint16_t instruction) {
	assert((instruction & 0xF0FF) == 0xF007);

	emu->registers[nibble3(instruction)] = emu->delayTimer;

	return INST_SUCCESS_INCR_PC;
}

int cif_lk(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_del(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_snd(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_ii(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_font(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_bcd(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_save(ChipEmu emu, uint16_t instruction) {
	return 0;
}

int cif_rest(ChipEmu emu, uint16_t instruction) {
	return 0;
}

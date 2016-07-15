// R-Type instructions

#include "chemu/instructions.h"
#include "chemu/display.h"
#include "chemu/memory.h"

#define RESERVED emu->memory.reserved

// se - Skip next if equal
ChipInstResult cif_se(ChipEmu *emu, ChipInstDec inst) {

	if (RESERVED.regs[inst.r.rnum_src] == RESERVED.regs[inst.r.rnum_dest])
		RESERVED.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// move - sets register value to another register
ChipInstResult cif_move(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.r.rnum_dest] = RESERVED.regs[inst.r.rnum_src];

	return INST_SUCCESS_INCR_PC;
}

// or - bitwise or
ChipInstResult cif_or(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.r.rnum_dest] |= RESERVED.regs[inst.r.rnum_src];

	return INST_SUCCESS_INCR_PC;
}

// and - bitwise and
ChipInstResult cif_and(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.r.rnum_dest] &= RESERVED.regs[inst.r.rnum_src];

	return INST_SUCCESS_INCR_PC;
}

// xor - bitwise xor
ChipInstResult cif_xor(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[inst.r.rnum_dest] ^= RESERVED.regs[inst.r.rnum_src];

	return INST_SUCCESS_INCR_PC;
}

// add - addition
ChipInstResult cif_add(ChipEmu *emu, ChipInstDec inst) {

	uint8_t vx = inst.r.rnum_dest, vy = inst.r.rnum_src;
	uint8_t temp = RESERVED.regs[vx];
	RESERVED.regs[vx] += RESERVED.regs[vy];

	// carry detection
	RESERVED.regs[CARRY_REG] = (temp > RESERVED.regs[vx]) ? 1 : 0;

	return INST_SUCCESS_INCR_PC;
}

// sub - subtraction
ChipInstResult cif_sub(ChipEmu *emu, ChipInstDec inst) {

	ChipReg vx = inst.r.rnum_dest, vy = inst.r.rnum_src;
	ChipReg temp = RESERVED.regs[vx];
	RESERVED.regs[vx] -= RESERVED.regs[vy];

	// borrow detection
	RESERVED.regs[CARRY_REG] = (temp < RESERVED.regs[vx] ? 1 : 0);

	return INST_SUCCESS_INCR_PC;
}

// shr - shift right by 1
ChipInstResult cif_shr(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[CARRY_REG] = RESERVED.regs[inst.r.rnum_dest] & 1;
	RESERVED.regs[inst.r.rnum_dest] >>= 1;

	return INST_SUCCESS_INCR_PC;
}

// subn - subtraction, operands swapped
ChipInstResult cif_subn(ChipEmu *emu, ChipInstDec inst) {

	// xor swap, swap rnum_dest and rnum_src of inst
	inst.r.rnum_dest ^= inst.r.rnum_src;
	inst.r.rnum_src ^= inst.r.rnum_dest;
	inst.r.rnum_dest ^= inst.r.rnum_src;
	inst.r.literal_lo = 5;

	return cif_sub(emu, inst);
}

// shift left by 1
ChipInstResult cif_shl(ChipEmu *emu, ChipInstDec inst) {

	RESERVED.regs[CARRY_REG] = RESERVED.regs[inst.r.rnum_dest] >> 7;
	RESERVED.regs[inst.r.rnum_dest] <<= 1;

	return INST_SUCCESS_INCR_PC;
}

// sn - skip if not equal
ChipInstResult cif_sn(ChipEmu *emu, ChipInstDec inst) {

	if (RESERVED.regs[inst.r.rnum_dest] != RESERVED.regs[inst.r.rnum_src])
		RESERVED.pc += 2;

	return INST_SUCCESS_INCR_PC;
}

// draw - draw sprite
ChipInstResult cif_draw(ChipEmu *emu, ChipInstDec inst) {

    ChipSprite sprite;
    sprite.x = RESERVED.regs[inst.r.rnum_dest];
    sprite.y = RESERVED.regs[inst.r.rnum_src];
    sprite.rows = inst.r.literal_lo;

    ChipAddress addr = RESERVED.addrReg;
    for (int i = 0; i < sprite.rows; ++i) {
        sprite.data[i] = chipmem_read(&emu->memory, addr);
        if (++addr > CHIP_END) {
            // out of bounds error
            return INST_FAILURE;
        }
    }

    RESERVED.regs[CARRY_REG] = chipdisplay_draw(&RESERVED.display, sprite);

    return INST_SUCCESS_INCR_PC;

}

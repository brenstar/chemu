// R-Type instructions

#include "chemu/instructions.h"
#include "chemu/display.h"
#include "chemu/memory.h"
#include "chemu/emulation.h"

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

    ChipReg temp = RESERVED.regs[inst.r.rnum_dest];
    RESERVED.regs[inst.r.rnum_dest] += RESERVED.regs[inst.r.rnum_src];

    // carry detection
    RESERVED.regs[CARRY_REG] = temp > RESERVED.regs[inst.r.rnum_dest];

    return INST_SUCCESS_INCR_PC;
}

// If regs[vx] > regs[vy] then CARRY_REG is set 1, 0 otherwise (or NOT borrow)
// regs[dest] = regs[vx] - regs[vy]
#define cif_sub_helper(dest, vx, vy) \
    RESERVED.regs[CARRY_REG] = RESERVED.regs[vx] > RESERVED.regs[vy]; \
    RESERVED.regs[dest] = RESERVED.regs[vx] - RESERVED.regs[vy]

// sub - subtraction, dest minus src
ChipInstResult cif_sub(ChipEmu *emu, ChipInstDec inst) {

    // dest = dest - src
    cif_sub_helper(inst.r.rnum_dest, inst.r.rnum_dest, inst.r.rnum_src);

    return INST_SUCCESS_INCR_PC;
}

// shr - shift right by 1
ChipInstResult cif_shr(ChipEmu *emu, ChipInstDec inst) {

    RESERVED.regs[CARRY_REG] = RESERVED.regs[inst.r.rnum_dest] & 1;
    RESERVED.regs[inst.r.rnum_dest] >>= 1;

    return INST_SUCCESS_INCR_PC;
}

// subn - subtraction, src minus dest
ChipInstResult cif_subn(ChipEmu *emu, ChipInstDec inst) {

    // dest = src - dest
    cif_sub_helper(inst.r.rnum_dest, inst.r.rnum_src, inst.r.rnum_dest);

    return INST_SUCCESS_INCR_PC;
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
    //emu->flags |= CHIP_REDRAW_FLAG;
    chipemu_redraw(emu);

    return INST_SUCCESS_INCR_PC;

}

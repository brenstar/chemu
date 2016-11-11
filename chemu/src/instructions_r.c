// R-Type instructions

#include "chemu/instructions.h"
#include "chemu/display.h"
#include "chemu/memory.h"
#include "chemu/emulation.h"

// se - Skip next if equal
ChipInstResult chemu_if_se(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    if (mem->reserved.regs[inst.r.rnum_src] == mem->reserved.regs[inst.r.rnum_dest])
        mem->reserved.pc += 2;

    return INST_SUCCESS_INCR_PC;
}

// move - sets register value to another register
ChipInstResult chemu_if_move(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.r.rnum_dest] = mem->reserved.regs[inst.r.rnum_src];

    return INST_SUCCESS_INCR_PC;
}

// or - bitwise or
ChipInstResult chemu_if_or(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.r.rnum_dest] |= mem->reserved.regs[inst.r.rnum_src];

    return INST_SUCCESS_INCR_PC;
}

// and - bitwise and
ChipInstResult chemu_if_and(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.r.rnum_dest] &= mem->reserved.regs[inst.r.rnum_src];

    return INST_SUCCESS_INCR_PC;
}

// xor - bitwise xor
ChipInstResult chemu_if_xor(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[inst.r.rnum_dest] ^= mem->reserved.regs[inst.r.rnum_src];

    return INST_SUCCESS_INCR_PC;
}

// add - addition
ChipInstResult chemu_if_add(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    ChipReg temp = mem->reserved.regs[inst.r.rnum_dest];
    mem->reserved.regs[inst.r.rnum_dest] += mem->reserved.regs[inst.r.rnum_src];

    // carry detection
    mem->reserved.regs[CARRY_REG] = temp > mem->reserved.regs[inst.r.rnum_dest];

    return INST_SUCCESS_INCR_PC;
}

// If regs[vx] > regs[vy] then CARRY_REG is set 1, 0 otherwise (or NOT borrow)
// regs[dest] = regs[vx] - regs[vy]
#define cif_sub_helper(dest, vx, vy) \
    mem->reserved.regs[CARRY_REG] = mem->reserved.regs[vx] > mem->reserved.regs[vy]; \
    mem->reserved.regs[dest] = mem->reserved.regs[vx] - mem->reserved.regs[vy]

// sub - subtraction, dest minus src
ChipInstResult chemu_if_sub(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    // dest = dest - src
    cif_sub_helper(inst.r.rnum_dest, inst.r.rnum_dest, inst.r.rnum_src);

    return INST_SUCCESS_INCR_PC;
}

// shr - shift right by 1
ChipInstResult chemu_if_shr(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[CARRY_REG] = mem->reserved.regs[inst.r.rnum_dest] & 1;
    mem->reserved.regs[inst.r.rnum_dest] >>= 1;

    return INST_SUCCESS_INCR_PC;
}

// subn - subtraction, src minus dest
ChipInstResult chemu_if_subn(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    // dest = src - dest
    cif_sub_helper(inst.r.rnum_dest, inst.r.rnum_src, inst.r.rnum_dest);

    return INST_SUCCESS_INCR_PC;
}

// shift left by 1
ChipInstResult chemu_if_shl(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    mem->reserved.regs[CARRY_REG] = mem->reserved.regs[inst.r.rnum_dest] >> 7;
    mem->reserved.regs[inst.r.rnum_dest] <<= 1;

    return INST_SUCCESS_INCR_PC;
}

// sn - skip if not equal
ChipInstResult chemu_if_sn(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {
    (void)emu;
    if (mem->reserved.regs[inst.r.rnum_dest] != mem->reserved.regs[inst.r.rnum_src])
        mem->reserved.pc += 2;

    return INST_SUCCESS_INCR_PC;
}

// draw - draw sprite
ChipInstResult chemu_if_draw(ChipEmu emu, ChipMem *mem, ChipInstDec inst) {

    ChipSprite sprite;
    sprite.x = mem->reserved.regs[inst.r.rnum_dest];
    sprite.y = mem->reserved.regs[inst.r.rnum_src];
    sprite.rows = inst.r.literal_lo;

    ChipAddress addr = mem->reserved.addrReg;
    for (int i = 0; i < sprite.rows; ++i) {
        sprite.data[i] = chemu_mem_read(mem, addr);
        if (++addr > CHIP_END) {
            // out of bounds error
            return INST_FAILURE;
        }
    }

    mem->reserved.regs[CARRY_REG] = chemu_disp_draw(&mem->reserved.display, sprite);
    //emu->flags |= CHIP_REDRAW_FLAG;
    chemu_emu_redraw(emu);

    return INST_SUCCESS_INCR_PC;

}

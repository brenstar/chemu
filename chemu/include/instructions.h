#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

//#include "chiptypes.h"

#include <stdint.h>

#include "ChipEmu.h"
#include "ChipInst.h"

// successful instruction execution
#define INST_SUCCESS            0
// successful instruction execution, increment program counter
#define INST_SUCCESS_INCR_PC    1
// instruction failed to execute
#define INST_FAILURE            2

int cif_sys(ChipEmu *emu, ChipInst instruction);

int cif_cls(ChipEmu *emu, ChipInst instruction);

int cif_ret(ChipEmu *emu, ChipInst instruction);

int cif_j(ChipEmu *emu, ChipInst instruction);

int cif_call(ChipEmu *emu, ChipInst instruction);

int cif_sei(ChipEmu *emu, ChipInst instruction);

int cif_sni(ChipEmu *emu, ChipInst instruction);

int cif_se(ChipEmu *emu, ChipInst instruction);

int cif_li(ChipEmu *emu, ChipInst instruction);

int cif_addi(ChipEmu *emu, ChipInst instruction);

int cif_move(ChipEmu *emu, ChipInst instruction);

int cif_or(ChipEmu *emu, ChipInst instruction);

int cif_and(ChipEmu *emu, ChipInst instruction);

int cif_xor(ChipEmu *emu, ChipInst instruction);

int cif_add(ChipEmu *emu, ChipInst instruction);

int cif_sub(ChipEmu *emu, ChipInst instruction);

int cif_shr(ChipEmu *emu, ChipInst instruction);

int cif_subn(ChipEmu *emu, ChipInst instruction);

int cif_shl(ChipEmu *emu, ChipInst instruction);

int cif_sn(ChipEmu *emu, ChipInst instruction);

int cif_la(ChipEmu *emu, ChipInst instruction);

int cif_jo(ChipEmu *emu, ChipInst instruction);

int cif_rnd(ChipEmu *emu, ChipInst instruction);

int cif_draw(ChipEmu *emu, ChipInst instruction);

int cif_sip(ChipEmu *emu, ChipInst instruction);

int cif_snip(ChipEmu *emu, ChipInst instruction);

int cif_ld(ChipEmu *emu, ChipInst instruction);

int cif_lk(ChipEmu *emu, ChipInst instruction);

int cif_del(ChipEmu *emu, ChipInst instruction);

int cif_snd(ChipEmu *emu, ChipInst instruction);

int cif_ii(ChipEmu *emu, ChipInst instruction);

int cif_font(ChipEmu *emu, ChipInst instruction);

int cif_bcd(ChipEmu *emu, ChipInst instruction);

int cif_save(ChipEmu *emu, ChipInst instruction);

int cif_rest(ChipEmu *emu, ChipInst instruction);

#endif

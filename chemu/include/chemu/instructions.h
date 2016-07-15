#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

//#include "chiptypes.h"

#include <stdint.h>

#include "ChipEmu.h"
#include "ChipInstDec.h"

// // successful instruction execution
// #define INST_SUCCESS            0
// // successful instruction execution, increment program counter
// #define INST_SUCCESS_INCR_PC    1
// // instruction failed to execute
// #define INST_FAILURE            2

ChipInstResult cif_sys(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_cls(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_ret(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_j(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_call(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_sei(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_sni(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_se(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_li(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_addi(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_move(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_or(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_and(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_xor(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_add(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_sub(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_shr(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_subn(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_shl(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_sn(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_la(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_jo(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_rnd(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_draw(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_sip(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_snip(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_ld(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_lk(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_del(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_snd(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_ii(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_font(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_bcd(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_save(ChipEmu *emu, ChipInstDec instruction);

ChipInstResult cif_rest(ChipEmu *emu, ChipInstDec instruction);

#endif

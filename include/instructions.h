#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "chiptypes.h"
#include <stdint.h>

#define INST_SUCCESS            0
#define INST_SUCCESS_INCR_PC    1
#define INST_FAILURE            2




int cif_sys(ChipEmu emu, uint16_t instruction);

int cif_cls(ChipEmu emu, uint16_t instruction);

int cif_ret(ChipEmu emu, uint16_t instruction);

int cif_j(ChipEmu emu, uint16_t instruction);

int cif_call(ChipEmu emu, uint16_t instruction);

int cif_sei(ChipEmu emu, uint16_t instruction);

int cif_sni(ChipEmu emu, uint16_t instruction);

int cif_se(ChipEmu emu, uint16_t instruction);

int cif_li(ChipEmu emu, uint16_t instruction);

int cif_addi(ChipEmu emu, uint16_t instruction);

int cif_move(ChipEmu emu, uint16_t instruction);

int cif_or(ChipEmu emu, uint16_t instruction);

int cif_and(ChipEmu emu, uint16_t instruction);

int cif_xor(ChipEmu emu, uint16_t instruction);

int cif_add(ChipEmu emu, uint16_t instruction);

int cif_sub(ChipEmu emu, uint16_t instruction);

int cif_shr(ChipEmu emu, uint16_t instruction);

int cif_subn(ChipEmu emu, uint16_t instruction);

int cif_shl(ChipEmu emu, uint16_t instruction);

int cif_sn(ChipEmu emu, uint16_t instruction);

int cif_la(ChipEmu emu, uint16_t instruction);

int cif_jo(ChipEmu emu, uint16_t instruction);

int cif_rnd(ChipEmu emu, uint16_t instruction);

int cif_draw(ChipEmu emu, uint16_t instruction);

int cif_sip(ChipEmu emu, uint16_t instruction);

int cif_snip(ChipEmu emu, uint16_t instruction);

int cif_ld(ChipEmu emu, uint16_t instruction);

int cif_lk(ChipEmu emu, uint16_t instruction);

int cif_del(ChipEmu emu, uint16_t instruction);

int cif_snd(ChipEmu emu, uint16_t instruction);

int cif_ii(ChipEmu emu, uint16_t instruction);

int cif_font(ChipEmu emu, uint16_t instruction);

int cif_bcd(ChipEmu emu, uint16_t instruction);

int cif_save(ChipEmu emu, uint16_t instruction);

int cif_rest(ChipEmu emu, uint16_t instruction);

#endif

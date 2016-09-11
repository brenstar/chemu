#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include <stdint.h>

#include "ChipEmu.h"
#include "ChipInstDec.h"
#include "internal/export.h"

CHEMU_API ChipInstResult cif_sys(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_cls(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ret(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_j(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_call(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sei(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sni(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_se(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_li(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_addi(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_move(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_or(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_and(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_xor(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_add(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sub(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_shr(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_subn(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_shl(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sn(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_la(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_jo(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_rnd(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_draw(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sip(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_snip(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ld(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_lk(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_del(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_snd(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ii(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_font(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_bcd(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_save(ChipEmu *emu, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_rest(ChipEmu *emu, ChipInstDec instruction);

#endif

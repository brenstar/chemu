#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include <stdint.h>

#include "ChipEmu.h"
#include "ChipMem.h"
#include "ChipInstDec.h"
#include "internal/export.h"

CHEMU_API ChipInstResult cif_sys(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_cls(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ret(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_j(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_call(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sei(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sni(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_se(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_li(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_addi(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_move(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_or(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_and(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_xor(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_add(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sub(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_shr(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_subn(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_shl(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sn(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_la(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_jo(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_rnd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_draw(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_sip(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_snip(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ld(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_lk(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_del(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_snd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_ii(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_font(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_bcd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_save(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult cif_rest(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

#endif

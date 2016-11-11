#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include <stdint.h>

#include "ChipEmu.h"
#include "ChipMem.h"
#include "ChipInstResult.h"
#include "ChipInstDec.h"
#include "internal/export.h"

CHEMU_API ChipInstResult chemu_if_sys(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_cls(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_ret(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_j(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_call(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_sei(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_sni(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_se(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_li(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_addi(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_move(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_or(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_and(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_xor(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_add(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_sub(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_shr(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_subn(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_shl(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_sn(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_la(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_jo(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_rnd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_draw(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_sip(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_snip(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_ld(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_lk(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_del(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_snd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_ii(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_font(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_bcd(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_save(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

CHEMU_API ChipInstResult chemu_if_rest(ChipEmu emu, ChipMem *mem, ChipInstDec instruction);

#endif

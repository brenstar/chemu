#ifndef _EMULATION_H
#define _EMULATION_H

#include "ChipEmu.h"
#include "instructions.h"


ChipEmu* chipemu_create();

int chipemu_mainLoop(ChipEmu *emu);

ChipInstFunc chipemu_decode(ChipInst instruction);

void chipemu_destroy(ChipEmu *emu);

void chipemu_reset(ChipEmu *emu);

#endif

#ifndef _EMULATION_H
#define _EMULATION_H

#include "ChipEmu.h"
#include "instructions.h"

#define CHIP_STEP_SUCCESS 0
#define CHIP_STEP_FAILURE 1




ChipEmu* chipemu_create();

int chipemu_mainLoop(ChipEmu *emu);

ChipInstFunc chipemu_decode(ChipInst instruction);

void chipemu_destroy(ChipEmu *emu);

void chipemu_reset(ChipEmu *emu);

int chipemu_step(ChipEmu *emu);

#endif

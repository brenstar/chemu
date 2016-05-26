#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>
#include "chiptypes.h"



ChipEmu chipemu_create();

int chipemu_mainLoop(ChipEmu emu);

ChipInstFunc chipemu_decode(uint16_t instruction);

void chipemu_destroy(ChipEmu emu);


#endif

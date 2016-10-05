
#ifndef _CHIPDP_H
#define _CHIPDP_H

#include <stdint.h>

#include "ChipAddress.h"
#include "ChipReg.h"

#define CHIP_REG_COUNT 16

// CHIP-8 Datapath structure
typedef struct ChipDP_s {
    ChipAddress pc;               // program counter
    ChipAddress addrReg;          // address Register I
    ChipReg regs[CHIP_REG_COUNT]; // data registers v0 - vF
    ChipReg sndTimer;             // sound timer
    ChipReg delTimer;             // delay timer
} ChipDP;


#endif

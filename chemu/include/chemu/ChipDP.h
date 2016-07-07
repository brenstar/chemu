#ifndef _CHIPDP_H
#define _CHIPDP_H

#include <stdint.h>

#include "ChipAddress.h"
#include "ChipReg.h"

// CHIP-8 Datapath structure
typedef struct ChipDP_s {
    ChipAddress pc;         // program counter
    ChipAddress addrReg;    // address Register I
    ChipReg regs[16];       // data registers v0 - vF
    ChipReg sndTimer;       // sound timer
    ChipReg delTimer;       // delay timer
} ChipDP;


#endif

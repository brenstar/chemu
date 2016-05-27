#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>

#include "ChipDP.h"
#include "ChipMem.h"
#include "ChipStack.h"

typedef struct ChipEmu_s {
    ChipDP dp;
    ChipStack stack;
    ChipMem memory;
} ChipEmu;





#endif

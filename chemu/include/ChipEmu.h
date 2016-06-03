#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>

#include "ChipDP.h"
#include "ChipMem.h"
#include "ChipStack.h"
#include "ChipInput.h"

typedef void (*DisplayRedrawCallback)();

typedef void (*PollInputCallback)(ChipInput *input);

typedef struct ChipEmu_s {
    ChipDP dp;
    ChipStack stack;
    ChipMem memory;
    ChipInput input;
} ChipEmu;





#endif

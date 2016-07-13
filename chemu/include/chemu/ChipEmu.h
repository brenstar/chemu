#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>

#include "ChipMem.h"
//#include "ChipStack.h"
//#include "ChipInput.h"
#include "ChipDrawCallback.h"

typedef void (*DisplayRedrawCallback)();

// callback for polling input
typedef void (*PollInputHandler)(ChipInput *input);

// callback called when a key press is awaited
typedef ChipKey (*PollKeyHandler)();

typedef struct ChipEmu_s {
    //ChipDP dp;
    //ChipStack stack;
    ChipMem memory;
    //ChipInput input;
    PollKeyHandler pollKeyHandler;
    PollInputHandler pollInputHandler;
    ChipDrawCallback drawCallback;
} ChipEmu;

#endif

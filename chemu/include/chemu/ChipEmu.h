#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>

#include "ChipMem.h"
//#include "ChipStack.h"
//#include "ChipInput.h"
//#include "ChipDrawCallback.h"
#include "ChipRedrawCallback.h"

#define CARRY_REG 15


typedef void (*DisplayRedrawCallback)();

// callback for polling input
typedef void (*PollInputHandler)(ChipInput *input);

// callback called when a key press is awaited
typedef ChipKey (*PollKeyHandler)();

typedef struct ChipEmu_s {
    ChipMem memory;
    bool running;
    PollKeyHandler pollKeyHandler;
    ChipRedrawCallback redrawCallback;
    //ChipDrawCallback drawCallback;
} ChipEmu;

#endif

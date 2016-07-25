#ifndef _CHIPEMU_H
#define _CHIPEMU_H

#include <stdint.h>
#include <stdbool.h>

#include "ChipMem.h"
#include "ChipTimer.h"
//#include "ChipStack.h"
//#include "ChipInput.h"
//#include "ChipDrawCallback.h"

#define CARRY_REG 15

#define CHIPEMU_DEFAULT_SPEED 500

typedef struct ChipEmu_s ChipEmu;

// #define CHIPEMU_RUNNING_FLAG 1
// #define CHIPEMU_SET_ST_FLAG  2 // set sound timer flag
// #define CHIPEMU_SET_DT_FLAG  4 // set delay timer flag
//
// #define flagget(emu, flag) ((emu->flags & flag) == flag)
// #define flagset(emu, flag) emu->flags |= flag;
// #define flagreset(emu, flag) emu->flags |= ~flag;


// callback called when a key press is awaited
typedef ChipKey (*PollKeyHandler)(ChipEmu*);

typedef void (*ChipRedrawCallback)(ChipEmu*);

struct ChipEmu_s {
    ChipMem memory;
    bool running;
    // execution speed in instructions per second (0 for no limit)
    unsigned int speed;
    ChipTimer soundTimer;
    ChipTimer delayTimer;
    PollKeyHandler pollKeyHandler;
    ChipRedrawCallback redrawCallback;
    //ChipDrawCallback drawCallback;
};

#endif

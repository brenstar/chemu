#ifndef _CHIPEMU_H
#define _CHIPEMU_H

//#include <stdint.h>
//#include <stdbool.h>

//#include "ChipMem.h"
//#include "ChipTimer.h"
//#include "ChipStack.h"
//#include "ChipInput.h"
//#include "ChipDrawCallback.h"

#define CARRY_REG 15

#define CHIPEMU_DEFAULT_SPEED 500

typedef struct ChipEmu_s *ChipEmu;

/*

// callback called when a key press is awaited
typedef ChipKey (*PollKeyHandler)(ChipEmu*);

#include "ChipRedrawCallback.h"
//typedef void (*ChipRedrawCallback)(ChipEmu*);

struct ChipEmu_s {
    ChipMem memory;
    bool running;
    // execution speed in instructions per second (0 for no limit)
    unsigned int speed;
    PollKeyHandler pollKeyHandler;
    ChipRedrawCallback redrawCallback;
    //ChipDrawCallback drawCallback;
};
*/
#endif

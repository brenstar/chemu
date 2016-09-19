#ifndef _CHIPTIMER_H
#define _CHIPTIMER_H

typedef struct ChipTimer_s * ChipTimer;

/*
#include "threads.h"

typedef struct ChipTimer_s {
    thrd_t thread;
    int running;
    void (*trigger)(void*);
} ChipTimer;

#define CHIPTIMER_INIT {.running = false, .trigger = NULL}
*/

#endif

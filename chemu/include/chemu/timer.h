#ifndef _TIMER_H
#define _TIMER_H

#include "ChipTimer.h"

// timer interval in microseconds (60Hz ~ 16,666 microseconds)
#define CHIP_TIMER_INTERVAL_US 16666

// timer interval in milleseconds (60Hz ~ 17 ms)
#define CHIP_TIMER_INTERVAL_MS 17

ChipTimer chiptimer_create(int initialValue);

void chiptimer_destroy(ChipTimer timer);

int chiptimer_get(ChipTimer timer);

void chiptimer_set(ChipTimer timer, int value);

bool chiptimer_start(ChipTimer timer);

void chiptimer_stop(ChipTimer timer);


#endif

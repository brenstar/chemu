#ifndef _TIMER_H
#define _TIMER_H

#ifndef _CHIPTIMER_IMPL
#define _CHIPTIMER_IMPL
typedef struct {} *ChipTimer;
#endif

// timer interval in microseconds (60Hz ~ 16,666 microseconds)
#define CHIP_TIMER_INTERVAL 16666

ChipTimer timer_create(int initialValue);

void timer_destroy(ChipTimer timer);

int timer_get(ChipTimer timer);

void timer_set(ChipTimer timer, int value);

void timer_start(ChipTimer timer);

void timer_stop(ChipTimer timer);


#endif

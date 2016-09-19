#include "chemu/timer.h"
#include "threads.h"

#include <stdlib.h>

struct ChipTimer_s {
    thrd_t thread;
    bool running;
    ChipTimerCallback callback;
    void *callbackArg;
};

static int timerThreadFunc(void *arg);


ChipTimer chiptimer_start(ChipTimerCallback callback, void *callbackArg) {
    ChipTimer timer = (ChipTimer)malloc(sizeof(struct ChipTimer_s));
    timer->callback = callback;
    timer->callbackArg = callbackArg;

    if (timer != NULL) {
        timer->running = true;
        int res = thrd_create(&timer->thread, timerThreadFunc, timer);
        if (res != thrd_success) {
            free(timer);
            timer = NULL;
        }
    }

    return timer;
}

void chiptimer_stop(ChipTimer timer) {
    if (!timer->running)
        return;

    timer->running = false;
    thrd_join(timer->thread, NULL);

    free(timer);
}



int timerThreadFunc(void *arg) {

    ChipTimer timer = (ChipTimer)arg;
    // cache timer struct members
    const struct timespec sleepTime = { .tv_sec = 0,
                                        .tv_nsec = CHIP_TIMER_INTERVAL_NS };
    const ChipTimerCallback callback = timer->callback;
    const void *callbackArg = timer->callbackArg;

    do {
        thrd_sleep(&sleepTime, NULL);
        // invoke callback
        callback(callbackArg);
    } while (timer->running);

    return 0;
}

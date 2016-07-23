#if __STDC_VERSION__ >= 199901L
#   define _XOPEN_SOURCE 600
#else
#   define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#ifdef __unix__
    #include <pthread.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define _CHIPTIMER_IMPL
typedef struct ChipTimer_s {
    int value;
    bool running;
    #ifdef __unix__
        pthread_t thread;
        pthread_mutex_t mutex;
    #endif
} *ChipTimer;

#include "chemu/timer.h"



#ifdef __unix__
    #define timerMutexLock(timer) pthread_mutex_lock(&timer->mutex)
    #define timerMutexUnlock(timer) pthread_mutex_unlock(&timer->mutex)
#else
    #define timerMutexLock(timer)
    #define timerMutexUnlock(timer)
#endif

static void* timerloop(void* arg);


ChipTimer chiptimer_create(int initialValue) {
    ChipTimer timer = (ChipTimer)malloc(sizeof(struct ChipTimer_s));
    timer->value = initialValue;
    timer->running = false;

    #ifdef __unix__
        pthread_mutex_init(&timer->mutex, NULL);
    #endif

    return timer;
}

void chiptimer_destroy(ChipTimer timer) {
    if (timer->running)
        chiptimer_stop(timer);
    #ifdef __unix__
        pthread_mutex_destroy(&timer->mutex);
    #endif
    free(timer);
}

int chiptimer_get(ChipTimer timer) {
    int val;

    timerMutexLock(timer);
    val = timer->value;
    timerMutexUnlock(timer);

    return val;
}

void chiptimer_set(ChipTimer timer, int value) {
    timerMutexLock(timer);
    timer->value = value;
    timerMutexUnlock(timer);
}

void chiptimer_start(ChipTimer timer) {
    if (timer->running)
        return; // timer already running

    timer->running = true;

    #ifdef __unix__
        if (pthread_create(&timer->thread, NULL, timerloop, timer) != 0)
            return; // failed to create thread
    #endif
}

void chiptimer_stop(ChipTimer timer) {
    if (!timer->running)
        return; // timer not running, no thread to join

    timer->running = false;

    #ifdef __unix__
        //void *result;
        pthread_join(timer->thread, NULL);
    #endif
}

static void* timerloop(void* arg) {
    ChipTimer timer = (ChipTimer)arg;
    do {
        usleep(CHIP_TIMER_INTERVAL);

        timerMutexLock(timer);
        if (timer->value > 0)
            --timer->value;
        timerMutexUnlock(timer);

    } while (timer->running);

    return NULL;
}

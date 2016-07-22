#ifdef __unix__
    #include <pthread.h>
#endif

#include <stdbool.h>

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
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


#ifdef __unix__
    #define timerMutexLock(timer) pthread_mutex_lock(&timer->mutex)
    #define timerMutexUnlock(timer) pthread_mutex_unlock(&timer->mutex)
#else
    #define timerMutexLock(timer)
    #define timerMutexUnlock(timer)
#endif

static void* timerloop(void* arg);


ChipTimer timer_create(int initialValue) {
    ChipTimer timer = (ChipTimer)malloc(sizeof(struct ChipTimer_s));
    timer->value = initialValue;
    timer->running = false;

    #ifdef __unix__
        pthread_mutex_init(&timer->mutex, NULL);
    #endif

    return timer;
}

void timer_destroy(ChipTimer timer) {
    if (timer->running)
        timer_stop(timer);
    #ifdef __unix__
        pthread_mutex_destroy(&timer->mutex);
    #endif
    free(timer);
}

int timer_get(ChipTimer timer) {
    int val;

    timerMutexLock(timer);
    val = timer->value;
    timerMutexUnlock(timer);

    return val;
}

void timer_set(ChipTimer timer, int value) {
    timerMutexLock(timer);
    timer->value = value;
    timerMutexUnlock(timer);
}

void timer_start(ChipTimer timer) {
    timer->running = true;

    #ifdef __unix__
        if (pthread_create(&timer->thread, NULL, timerloop, timer) != 0)
            return; // failed to create thread
    #endif
}

void timer_stop(ChipTimer timer) {
    timer->running = false;

    #ifdef __unix__
        pthread_cancel(timer->thread);
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

        printf("Timer value: %d\n", timer->value);
    } while (timer->running);

    return NULL;
}

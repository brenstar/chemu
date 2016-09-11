#include "chemu/timer.h"

#if defined(__unix__)
    #if __STDC_VERSION__ >= 199901L
    #   define _XOPEN_SOURCE 600
    #else
    #   define _XOPEN_SOURCE 500
    #endif /* __STDC_VERSION__ */

    #include <pthread.h>
    #include <unistd.h>

    #define TIMER_STRUCT_MEMBERS pthread_t thread; \
                                 pthread_mutex_t mutex;

    #define timerMutexLock(timer) pthread_mutex_lock(&timer->mutex)
    #define timerMutexUnlock(timer) pthread_mutex_unlock(&timer->mutex)
    #define timerSleep() usleep(CHIP_TIMER_INTERVAL_US)

    #define timerMutexInit(timer) pthread_mutex_init(&timer->mutex, NULL)
    #define timerMutexDestroy(timer) pthread_mutex_destroy(&timer->mutex)

    #define timerThreadCreate(timer, failed) \
        if (pthread_create(&timer->thread, NULL, timerloop, timer) != 0) \
            failed = true

    #define timerThreadJoin(timer) pthread_join(timer->thread, NULL);


    #define TIMERLOOP_RETURN_TYPE void*
    #define TIMERLOOP_PARAM_TYPE void*
    #define TIMERLOOP_RETURN_VALUE NULL

#elif defined(_WIN32)

    #include <windows.h>

    #define TIMER_STRUCT_MEMBERS DWORD threadId; \
                                 HANDLE thread; \
                                 HANDLE mutex;

    #define timerMutexLock(timer) WaitForSingleObject(timer->mutex, INFINITE)
    #define timerMutexUnlock(timer) ReleaseMutex(timer->mutex)
    #define timerSleep() Sleep(CHIP_TIMER_INTERVAL_MS)

    #define timerMutexInit(timer) timer->mutex = CreateMutex(NULL, FALSE, NULL)

    #define timerMutexDestroy(timer) CloseHandle(timer->mutex)

    #define timerThreadCreate(timer, failed) \
        timer->thread = CreateThread(NULL, 0, timerloop, timer, 0, &timer->threadId); \
        failed = timer->thread == NULL

    #define timerThreadJoin(timer) WaitForSingleObject(timer->thread, INFINITE)

    #define TIMERLOOP_RETURN_TYPE DWORD WINAPI
    #define TIMERLOOP_PARAM_TYPE LPVOID
    #define TIMERLOOP_RETURN_VALUE 0

#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


struct ChipTimer_s {
    int value;
    bool running;
    TIMER_STRUCT_MEMBERS
};



static TIMERLOOP_RETURN_TYPE timerloop(TIMERLOOP_PARAM_TYPE param);


ChipTimer chiptimer_create(int initialValue) {
    ChipTimer timer = (ChipTimer)malloc(sizeof(struct ChipTimer_s));
    timer->value = initialValue;
    timer->running = false;

    // initialize the mutex
    timerMutexInit(timer);

    return timer;
}

void chiptimer_destroy(ChipTimer timer) {
    if (timer->running)
        chiptimer_stop(timer);

    // destroy mutex
    timerMutexDestroy(timer);

    free(timer);
}

int chiptimer_get(ChipTimer timer) {
    int val;

    // get timer value
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

bool chiptimer_start(ChipTimer timer) {
    if (timer->running)
        return false; // timer already running

    timer->running = true;

    // create thread
    bool failed = false;
    timerThreadCreate(timer, failed);

    if (failed) {
        timer->running = false;
        return false;
    }

    return true;
}

void chiptimer_stop(ChipTimer timer) {
    if (!timer->running)
        return; // timer not running, no thread to join

    timer->running = false;

    timerThreadJoin(timer);

}


static TIMERLOOP_RETURN_TYPE timerloop(TIMERLOOP_PARAM_TYPE param) {
    ChipTimer timer = (ChipTimer)param;
    do {
        timerSleep();

        timerMutexLock(timer);
        if (timer->value > 0)
            --timer->value;
        timerMutexUnlock(timer);
    } while (timer->running);

    return TIMERLOOP_RETURN_VALUE;
}

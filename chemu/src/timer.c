#if __STDC_VERSION__ >= 199901L
#   define _XOPEN_SOURCE 600
#else
#   define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#ifdef __unix__
    #include <pthread.h>
	#include <unistd.h>
#elif defined(_WIN32)
	#include <windows.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


struct ChipTimer_s {
    int value;
    bool running;
    #ifdef __unix__
        pthread_t thread;
        pthread_mutex_t mutex;
	#elif defined(_WIN32)
		DWORD threadId;
		HANDLE thread;
		HANDLE mutex;
	#endif
};

#include "chemu/timer.h"



#ifdef __unix__
    #define timerMutexLock(timer) pthread_mutex_lock(&timer->mutex)
    #define timerMutexUnlock(timer) pthread_mutex_unlock(&timer->mutex)
	static void* timerloop(void* arg);
#else
    #define timerMutexLock(timer) WaitForSingleObject(timer->mutex, INFINITE)
    #define timerMutexUnlock(timer) ReleaseMutex(timer->mutex)
	static DWORD WINAPI timerloop(LPVOID lpParam);
#endif





ChipTimer chiptimer_create(int initialValue) {
    ChipTimer timer = (ChipTimer)malloc(sizeof(struct ChipTimer_s));
    timer->value = initialValue;
    timer->running = false;

    #ifdef __unix__
        pthread_mutex_init(&timer->mutex, NULL);
    #elif defined(_WIN32)
		timer->mutex = CreateMutex(NULL, FALSE, NULL);
	#endif

    return timer;
}

void chiptimer_destroy(ChipTimer timer) {
    if (timer->running)
        chiptimer_stop(timer);
    #ifdef __unix__
        pthread_mutex_destroy(&timer->mutex);
    #elif defined(_WIN32)
		CloseHandle(timer->mutex);
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

bool chiptimer_start(ChipTimer timer) {
	if (timer->running)
		return false; // timer already running

	timer->running = true;

    #ifdef __unix__
        if (pthread_create(&timer->thread, NULL, timerloop, timer) != 0) {
			timer->running = false;
            return false; // failed to create thread
		}
    #elif defined(_WIN32)
		timer->thread = CreateThread(NULL, 0, timerloop, timer, 0, &timer->threadId);
		if (timer->thread == NULL) {
			timer->running = false;
			return false;
		}
	#endif

	return true;
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


#define decrementTimer(timer) \
	if (timer->value > 0)     \
		--timer->value;


#if defined(__unix__)

static void* timerloop(void* arg) {
    ChipTimer timer = (ChipTimer)arg;
    do {
        usleep(CHIP_TIMER_INTERVAL_US);

        timerMutexLock(timer);
		decrementTimer(timer);
        timerMutexUnlock(timer);

    } while (timer->running);

    return NULL;
}
#elif defined(_WIN32)

static DWORD WINAPI timerloop(LPVOID lpParam) {
	ChipTimer timer = (ChipTimer)lpParam;

	do {
		Sleep(CHIP_TIMER_INTERVAL_MS);

		timerMutexLock(timer);
		decrementTimer(timer);
		timerMutexUnlock(timer);
	} while (timer->running);

	return 0;
}

#endif
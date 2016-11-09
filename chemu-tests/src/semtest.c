
#include "chemu/threads.h"
#include "chemu/threadutil.h"

#include <stdio.h>

static int testThread(void *arg);

int main() {

    ChipSemaphore sem;
    if (ctu_sem_init(&sem, 0) == thrd_error) {
        puts("failed to create semaphore");
        return 1;
    }


    puts("Creating thread");
    thrd_t thrd;
    thrd_create(&thrd, testThread, &sem);

    puts("Waiting for thread");
    ctu_sem_wait(&sem);

    ctu_sem_destroy(&sem);

    thrd_join(thrd, NULL);

    puts("Leaving main");
    return 0;
}

int testThread(void *arg) {
    ChipSemaphore *sem = (ChipSemaphore*)arg;

    puts("Doing work...");

    const struct timespec sleepTime = {.tv_sec = 2, .tv_nsec = 0};
    thrd_sleep(&sleepTime, NULL);

    puts("Signalling...");
    ctu_sem_signal(sem, NULL);

    return 0;
}

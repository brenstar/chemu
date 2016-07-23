#include "chemu/timer.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <time.h>

volatile sig_atomic_t flag = 0;

void interruptHandler(int sig) {
    flag = sig;
}

#define LOOP_SPEED 512

int main() {
    signal(SIGINT, interruptHandler);

    ChipTimer timer = chiptimer_create(255);
    chiptimer_start(timer);

    int counter = 0;
    // interval of time (in nanoseconds) each iteration of the loop will take
    const long int interval = 1000000000L / LOOP_SPEED;
    clock_t lastTime = clock();
    do {
        if (counter++ == LOOP_SPEED) {
            int timerVal = chiptimer_get(timer);
            printf("Timer value is %d\n", timerVal);
            if (timerVal == 0) {
                printf("Resetting timer\n");
                chiptimer_set(timer, 255);
            }
            counter = 0;
        }
        clock_t currentTime = clock();
        long int sleepTime = interval - ((currentTime - lastTime) * 1000000000L / CLOCKS_PER_SEC);
        lastTime = currentTime;

        if (sleepTime > 0) {
            struct timespec t;
            t.tv_sec = 0;
            t.tv_nsec = sleepTime;
            nanosleep(&t, NULL);
        }
    } while (flag == 0);

    chiptimer_stop(timer);
    chiptimer_destroy(timer);

    return 0;

}

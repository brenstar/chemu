#include "chemu/timer.h"

#include <stdio.h>
#include <time.h>


void timerTrigger(void* arg) {
    static int counter = 0;

    if (++counter == 60) {
        counter = 0;
        time_t tim;
        char buf[10];
        struct tm *tm_info;
        time(&tim);
        tm_info = localtime(&tim);

        strftime(buf, 10, "%H:%M:%S", tm_info);
        puts(buf);

    }
}


int main() {

    //ChipTimer timer = chiptimer_create(255);
    ChipTimer timer = chiptimer_start(timerTrigger, NULL);

    getchar();

    chiptimer_stop(timer);

    return 0;

/*
    //int counter = 0;
    // interval of time (in nanoseconds) each iteration of the loop will take
    //const long int interval = 1000000L / LOOP_SPEED;
    clock_t lastTime = clock();
    do {
        //if (counter++ == LOOP_SPEED) {
        if ((clock() - lastTime) / CLOCKS_PER_SEC > 0) {
            int timerVal = chiptimer_get(timer);
            printf("Timer value is %d\n", timerVal);
            if (timerVal == 0) {
                printf("Resetting timer\n");
                chiptimer_set(timer, 255);
            }
            lastTime = clock();
            //counter = 0;
        }
        // clock_t currentTime = clock();
        // long int sleepTime = interval - ((currentTime - lastTime) * 1000000L / CLOCKS_PER_SEC);
        // lastTime = currentTime;
        //
        // if (sleepTime > 0) {
        //     // struct timespec t;
        //     // t.tv_sec = 0;
        //     // t.tv_nsec = sleepTime;
        //     // nanosleep(&t, NULL);
        //     usleep(sleepTime);
        // }
    } while (flag == 0);

    chiptimer_stop(timer);
    chiptimer_destroy(timer);

    return 0;
*/

}

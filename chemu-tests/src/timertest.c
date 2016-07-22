#include "chemu/timer.h"

#include <stdio.h>

int main() {
    ChipTimer timer = timer_create(60);
    timer_start(timer);

    getchar();

    timer_stop(timer);
    timer_destroy(timer);

    return 0;

}

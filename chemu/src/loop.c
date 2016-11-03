
#include <stdlib.h>
#include <stdbool.h>

#include "chemu/ChipLoopState.h"
#include "chemu/emulation.h"
#include "chemu/loop.h"
#include "chemu/threads.h"
#include "chemu/timer.h"

struct ChipLoop_s {
    ChipEmu emu;
    ChipLoopState state;
    mtx_t lock;
    thrd_t thread;
    bool shouldBreak;
};

struct TimerHandlerArg_s {
    ChipEmu emu;
    int timerid;
};

static void __timerHandler(void *arg);

static int __loopMain(void *arg);


void chiploop_break(ChipLoop loop) {

}

ChipLoop chiploop_create(ChipEmu emu) {
    ChipLoop loop = (ChipLoop)malloc(sizeof(struct ChipLoop_s));

    loop->emu = emu;
    loop->state = CHIPLOOP_INACTIVE;
    loop->shouldBreak = false;

    mtx_init(&loop->lock, mtx_plain);

    return loop;
}

void chiploop_continue(ChipLoop loop) {

}

void chiploop_destroy(ChipLoop loop) {
    if (LOOPACTIVE(loop->state))
        chiploop_stop(loop);

    mtx_destroy(&loop->lock);
    free(loop);
}

int chiploop_main(ChipLoop loop) {
    return 0;
}

void chiploop_start(ChipLoop loop) {
    mtx_lock(&loop->lock);
    if (!LOOPACTIVE(loop->state)) {
        loop->state = CHIPLOOP_ACTIVE;
        thrd_create(&loop->thread, __loopMain, loop);
    }
    mtx_unlock(&loop->lock);
}

void chiploop_stop(ChipLoop loop) {

}

static int __loopMain(void *arg) {
    ChipLoop loop = (ChipLoop)arg;

    ChipTimer soundTimer = chiptimer_start(__timerHandler, NULL);
    ChipTimer delayTimer = chiptimer_start(__timerHandler, NULL);


    chiptimer_stop(soundTimer);
    chiptimer_stop(delayTimer);

    return 0;

}

static void __timerHandler(void* arg) {
    ChipLoop loop = (ChipLoop)arg;
    // decrement sound and delay timers
    chipemu_triggerDelayTimer(loop->emu);
    chipemu_triggerSoundTimer(loop->emu);

}

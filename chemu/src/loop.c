
#include <stdlib.h>
#include <stdbool.h>

#include "chemu/ChipLoopState.h"
#include "chemu/emulation.h"
#include "chemu/loop.h"
#include "chemu/threads.h"
#include "chemu/timer.h"
#include "chemu/logger.h"

#define LOOP_BREAK 0
#define LOOP_STOP 1

struct ChipLoop_s {
    ChipEmu emu;
    ChipLoopState state;
    mtx_t lock;
    thrd_t thread;
    // condition variable used to signal main loop to pause emulation
    // signaled in break and continue functions
    bool shouldBreak;
    bool shouldStop;
    cnd_t breakCV;
    mtx_t breakLock;
    bool stateChanging;
    cnd_t stateChangingCV;
    mtx_t stateChangingLock;
};

#define TIMERID_SOUND 0
#define TIMERID_DELAY 1

struct TimerHandlerArg_s {
    ChipEmu emu;
    int timerid;
};



static void __timerHandler(void *arg);

static int __loopMain(void *arg);

static void __waitForStateChange(ChipLoop loop);

static void __updateState(ChipLoop loop, ChipLoopState state);

void chiploop_break(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (loop->state == CHIPLOOP_ACTIVE) {
        mtx_lock(&loop->breakLock);
        loop->shouldBreak = true;
        mtx_unlock(&loop->breakLock);

        // wait until the loop breaks
        __waitForStateChange(loop);
    }

    mtx_unlock(&loop->lock);
}

ChipLoop chiploop_create(ChipEmu emu) {
    ChipLoop loop = (ChipLoop)malloc(sizeof(struct ChipLoop_s));

    loop->emu = emu;
    loop->state = CHIPLOOP_INACTIVE;
    loop->shouldBreak = false;
    loop->stateChanging = false;
    loop->shouldStop = false;

    mtx_init(&loop->lock, mtx_plain);
    mtx_init(&loop->stateChangingLock, mtx_plain);
    mtx_init(&loop->breakLock, mtx_plain);

    cnd_init(&loop->breakCV);
    cnd_init(&loop->stateChangingCV);

    return loop;
}

void chiploop_continue(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (loop->state == CHIPLOOP_IDLE) {
        mtx_lock(&loop->breakLock);
        loop->shouldBreak = false;
        cnd_signal(&loop->breakCV);
        mtx_unlock(&loop->breakLock);

        // possible race condion, if __updateState gets called before __waitForState
        // caller will wait endlessly
        __waitForStateChange(loop);
    }

    mtx_unlock(&loop->lock);
}

void chiploop_destroy(ChipLoop loop) {
    if (LOOPACTIVE(loop->state))
        chiploop_stop(loop);

    mtx_destroy(&loop->lock);
    mtx_destroy(&loop->stateChangingLock);
    mtx_destroy(&loop->breakLock);
    cnd_destroy(&loop->breakCV);
    cnd_destroy(&loop->stateChangingCV);

    free(loop);
}

int chiploop_main(ChipLoop loop) {
    return __loopMain(loop);
}

void chiploop_start(ChipLoop loop) {
    mtx_lock(&loop->lock);
    if (!LOOPACTIVE(loop->state)) {
        loop->state = CHIPLOOP_ACTIVE;
        int result = thrd_create(&loop->thread, __loopMain, loop);
        chiplog_info(" Starting loop, thread result: %d\n", result);
    }
    mtx_unlock(&loop->lock);
}

void chiploop_stop(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (LOOPACTIVE(loop->state)) {
        loop->shouldStop = true;
        __waitForStateChange(loop);
    }

    int exitCode;
    thrd_join(loop->thread, &exitCode);

    chiplog_info(" Loop stopped, exit code %d\n", exitCode);

    mtx_unlock(&loop->lock);
}

static int __loopMain(void *arg) {
    ChipLoop loop = (ChipLoop)arg;
    const ChipEmu emu = loop->emu;
    const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 1000000};

    struct TimerHandlerArg_s sndArg, delArg;
    sndArg.emu = loop->emu;
    delArg.emu = loop->emu;
    sndArg.timerid = TIMERID_SOUND;
    delArg.timerid = TIMERID_DELAY;

    ChipTimer soundTimer = chiptimer_start(__timerHandler, &sndArg);
    ChipTimer delayTimer = chiptimer_start(__timerHandler, &delArg);

    for (;;) {
        // emulate cycle
        int result = chipemu_step(emu);

        mtx_lock(&loop->stateChangingLock);
        bool stateChanging = loop->stateChanging;
        mtx_unlock(&loop->stateChangingLock);

        if (stateChanging) {
            if (loop->shouldBreak) {

                __updateState(loop, CHIPLOOP_IDLE);

                // wait until chiploop_continue is called
                mtx_lock(&loop->breakLock);
                while (loop->shouldBreak)
                    cnd_wait(&loop->breakCV, &loop->breakLock);
                mtx_unlock(&loop->breakLock);

                __updateState(loop, CHIPLOOP_ACTIVE);

            } else if (loop->shouldStop) {

                __updateState(loop, CHIPLOOP_INACTIVE);

                // exit loop
                break;
            }
        }

        thrd_sleep(&sleepTime, NULL);
        
        
    }

    chiptimer_stop(soundTimer);
    chiptimer_stop(delayTimer);


    return 0;

}

static void __timerHandler(void* arg) {
    struct TimerHandlerArg_s *targ = (struct TimerHandlerArg_s *)arg;
    if (targ->timerid == TIMERID_SOUND)
        chipemu_triggerSoundTimer(targ->emu);
    else
        chipemu_triggerDelayTimer(targ->emu);

}

void __waitForStateChange(ChipLoop loop) {
    mtx_lock(&loop->stateChangingLock);
    loop->stateChanging = true;
    while (!loop->stateChanging)
        cnd_wait(&loop->stateChangingCV, &loop->stateChangingLock);
    mtx_unlock(&loop->stateChangingLock);
}

void __updateState(ChipLoop loop, ChipLoopState state) {
    mtx_lock(&loop->stateChangingLock);
    loop->state = state;
    loop->stateChanging = false;
    cnd_signal(&loop->stateChangingCV);
    mtx_unlock(&loop->stateChangingLock);
}
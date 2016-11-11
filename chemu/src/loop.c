
#include <stdlib.h>
#include <stdbool.h>

#include "chemu/ChipLoopState.h"
#include "chemu/emulation.h"
#include "chemu/loop.h"
#include "chemu/threads.h"
#include "chemu/threadutil.h"
#include "chemu/timer.h"
#include "chemu/logger.h"

typedef enum {
    LOOPINT_NULL,
    LOOPINT_BREAK,
    LOOPINT_STOP
} LoopInterrupt;

struct ChipLoop_s {
    ChipEmu emu;
    ChipLoopState state;
    mtx_t lock;
    thrd_t thread;
    LoopInterrupt interrupt;
    mtx_t interruptLock;
    ChipSemaphore breakSem;
    ChipSemaphore stateChangeSem;
};



#define TIMERID_SOUND 0
#define TIMERID_DELAY 1

struct TimerHandlerArg_s {
    ChipEmu emu;
    int timerid;
};



static void __timerHandler(void *arg);

static int __loopMain(void *arg);


void chemu_loop_break(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (loop->state == CHIPLOOP_ACTIVE) {

        mtx_lock(&loop->interruptLock);
        loop->interrupt = LOOPINT_BREAK;
        mtx_unlock(&loop->interruptLock);

        // wait until the loop changes to the IDLE state
        ctu_sem_wait(&loop->stateChangeSem);

        
    }

    mtx_unlock(&loop->lock);
}

ChipLoop chemu_loop_create(ChipEmu emu) {
    ChipLoop loop = (ChipLoop)malloc(sizeof(struct ChipLoop_s));

    loop->emu = emu;
    loop->state = CHIPLOOP_INACTIVE;
    loop->interrupt = LOOPINT_NULL;

    mtx_init(&loop->lock, mtx_plain);
    mtx_init(&loop->interruptLock, mtx_plain);

    ctu_sem_init(&loop->breakSem, 0);
    ctu_sem_init(&loop->stateChangeSem, 0);

    return loop;
}

void chemu_loop_continue(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (loop->state == CHIPLOOP_IDLE) {

        mtx_lock(&loop->interruptLock);
        loop->interrupt = LOOPINT_NULL;
        mtx_unlock(&loop->interruptLock);

        ctu_sem_signal(&loop->breakSem, NULL);

        ctu_sem_wait(&loop->stateChangeSem);

    }

    mtx_unlock(&loop->lock);
}

void chemu_loop_destroy(ChipLoop loop) {
    if (LOOPACTIVE(loop->state))
        chemu_loop_stop(loop);

    mtx_destroy(&loop->lock);
    mtx_destroy(&loop->interruptLock);

    ctu_sem_destroy(&loop->stateChangeSem);

    free(loop);
}

int chemu_loop_main(ChipLoop loop) {
    return __loopMain(loop);
}

void chemu_loop_start(ChipLoop loop) {
    mtx_lock(&loop->lock);
    if (LOOPINACTIVE(loop->state)) {
        loop->state = CHIPLOOP_ACTIVE;
        int result = thrd_create(&loop->thread, __loopMain, loop);
        chiplog_info(" Starting loop, thread result: %d\n", result);
    }
    mtx_unlock(&loop->lock);
}

void chemu_loop_stop(ChipLoop loop) {
    mtx_lock(&loop->lock);

    if (LOOPACTIVE(loop->state)) {

        // set the interrupt to STOP
        mtx_lock(&loop->interruptLock);
        loop->interrupt = LOOPINT_STOP;
        mtx_unlock(&loop->interruptLock);

        if (loop->state == CHIPLOOP_IDLE) {
            ctu_sem_signal(&loop->breakSem, NULL);
            ctu_sem_wait(&loop->stateChangeSem);
        }

        ctu_sem_wait(&loop->stateChangeSem);
        
    }

    int exitCode;
    thrd_join(loop->thread, &exitCode);

    chiplog_info(" Loop stopped, exit code %d\n", exitCode);

    mtx_unlock(&loop->lock);
}

static int __loopMain(void *arg) {
    int error = 0;
    ChipLoop loop = (ChipLoop)arg;
    const ChipEmu emu = loop->emu;
    const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 3000000};

    struct TimerHandlerArg_s sndArg, delArg;
    sndArg.emu = loop->emu;
    delArg.emu = loop->emu;
    sndArg.timerid = TIMERID_SOUND;
    delArg.timerid = TIMERID_DELAY;

    ChipTimer soundTimer = chemu_timer_start(__timerHandler, &sndArg);
    ChipTimer delayTimer = chemu_timer_start(__timerHandler, &delArg);

    //int cycles = 0;

    for (;;) {
        // emulate cycle
        int result = chemu_emu_step(emu);
        if (result) {
            error = result;
            break;
        }

        // check for interrupts
        mtx_lock(&loop->interruptLock);
        LoopInterrupt interrupt = loop->interrupt;
        mtx_unlock(&loop->interruptLock);

        if (interrupt == LOOPINT_BREAK) {
            loop->state = CHIPLOOP_IDLE;
            ctu_sem_signal(&loop->stateChangeSem, NULL);

            ctu_sem_wait(&loop->breakSem);

            loop->state = CHIPLOOP_ACTIVE;
            ctu_sem_signal(&loop->stateChangeSem, NULL);
        }

        if (interrupt == LOOPINT_STOP) {
            loop->state = CHIPLOOP_INACTIVE;
            ctu_sem_signal(&loop->stateChangeSem, NULL);
            break;
        }

        //if (cycles++ == 4) {
            thrd_sleep(&sleepTime, NULL);
        //    cycles = 0;
        //}
        
        
    }

    chemu_timer_stop(soundTimer);
    chemu_timer_stop(delayTimer);


    return error;

}

static void __timerHandler(void* arg) {
    struct TimerHandlerArg_s *targ = (struct TimerHandlerArg_s *)arg;
    if (targ->timerid == TIMERID_SOUND)
        chemu_emu_triggerSoundTimer(targ->emu);
    else
        chemu_emu_triggerDelayTimer(targ->emu);

}
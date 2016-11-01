
#include <stdlib.h>

#include "chemu/ChipLoopState.h"
#include "chemu/loop.h"
#include "chemu/threads.h"

struct ChipLoop_s {
    ChipEmu emu;
    ChipLoopState state;
    mtx_t lock;
    thrd_t thread;
};

    


void chiploop_break(ChipLoop loop) {

}

ChipLoop chiploop_create(ChipEmu emu) {
    ChipLoop loop = (ChipLoop)malloc(sizeof(struct ChipLoop_s));

    loop->emu = emu;
    loop->state = CHIPLOOP_INACTIVE;

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
        thrd_create(&loop->thread, chiploop_main, loop);
    }
    mtx_unlock(&loop->lock);
}

void chiploop_stop(ChipLoop loop) {

}
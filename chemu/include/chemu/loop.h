#ifndef _LOOP_H
#define _LOOP_H

#include "ChipEmu.h"
#include "ChipLoop.h"

#include "internal/export.h"

//
// Signals the loop to pause execution. The loop handle must be active when
// calling this function otherwise this function does nothing. If sucessful,
// the loop will be in the idle state, which can be returned to the active
// state by calling chiploop_continue or to the inactive state by calling
// chiploop_stop.
//
CHEMU_API void chiploop_break(ChipLoop loop);

//
// Creates a loop handle with the given emulator handle.
// Loop handles have the following states:
//   1. inactive: default
//   2. active: main loop is running
//   3. idle: main loop is paused (break was called)
//
CHEMU_API ChipLoop chiploop_create(ChipEmu emu);

//
// Signals the loop to resume execution. The loop handle must be idle when
// calling this function otherwise this function does nothing.
//
CHEMU_API void chiploop_continue(ChipLoop loop);

//
// Frees all resources used by the given loop handle. If the loop is active
// when calling this function it will be stopped by a call to chiploop_stop.
//
CHEMU_API void chiploop_destroy(ChipLoop loop);

//
// Begins execution of the main loop in the calling thread. This function will
// loop and call chipemu_step repeatedly until an error occurs or if signaled
// by chiploop_stop. This function returns zero if signaled to stop and nonzero
// if an error occured.
//
CHEMU_API int chiploop_main(ChipLoop loop);

//
// Starts execution of the main loop in a separate thread. The loop handle must
// be inactive when calling this function, otherwise nothing happens.
//
CHEMU_API void chiploop_start(ChipEmu emu);

//
// Signals to stop execution of the main loop. The loop handle must be in the
// active or idle state when calling this function, otherwise nothing happens.
//
CHEMU_API void chiploop_stop(ChipLoop loop);

#endif

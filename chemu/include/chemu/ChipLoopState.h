#ifndef _CHIPLOOPSTATE_H
#define _CHIPLOOPSTATE_H

// macro determines if an emulator loop is active (state is IDLE or LOOP)
#define LOOPACTIVE(s) ((s & 1) == 1)

//
// Enum of possible states the ChipEmu handle can encounter. The value of each
// enum is a 2 bit number. The LSB of this number determines if the emulator
// has an active loop in a separate thread (LOOP, IDLE). The MSB determines if
// a breakpoint has occured (IDLE), which causes the loop to pause.
//
typedef enum ChipLoopState_e {
    CHIPLOOP_INACTIVE   = 0,    // 0b00 loop is ready to start
    CHIPLOOP_ACTIVE     = 1,    // 0b01 loop is active
    CHIPLOOP_IDLE       = 3,    // 0b11 loop is active, at breakpoint
    CHIPLOOP_ERROR      = 2,    // 0b10 error occured during emulation
} ChipLoopState;

#endif

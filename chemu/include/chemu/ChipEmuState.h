#ifndef _CHIPEMUSTATE_H
#define _CHIPEMUSTATE_H

// macro determines if an emulator loop is active (state is IDLE or LOOP)
#define LOOPACTIVE(s) ((s & 1) == 1)

//
// Enum of possible states the ChipEmu handle can encounter. The value of each
// enum is a 2 bit number. The LSB of this number determines if the emulator
// has an active loop in a separate thread (LOOP, IDLE). The MSB determines if
// a breakpoint has occured (IDLE), which causes the loop to pause.
//
typedef enum ChipEmuState_e {
    CHIPEMU_READY   = 0,    // 0b00 emulator is ready to step, or start a loop
    CHIPEMU_LOOP    = 1,    // 0b01 emulator is in a loop
    CHIPEMU_IDLE    = 3,    // 0b11 breakpoint
    CHIPEMU_ERROR   = 2,    // 0b10 error occured during step
    CHIPEMU_ILLEGAL         // emulator cannot perform requested operation in current state
} ChipEmuState;

#endif

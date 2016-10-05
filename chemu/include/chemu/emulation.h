#ifndef _EMULATION_H
#define _EMULATION_H

#include "ChipEmu.h"
#include "ChipEmuState.h"
#include "ChipInput.h"
#include "ChipInst.h"
#include "ChipInstResult.h"
#include "ChipDP.h"
#include "ChipStack.h"
#include "ChipDisplay.h"
#include "internal/export.h"

#define CHIP_STEP_SUCCESS 0
#define CHIP_STEP_FAILURE 1

#define CHIP_LOAD_FAILURE -1

//
// Signals the emulation thread to stop execution. The emulator must be in the
// LOOP state when calling this function, otherwise this function does nothing.
// If successful, the emulator will be in the IDLE state.
//
CHEMU_API ChipEmuState chipemu_break(ChipEmu emu);

//
// Creates an emulator handle. Callbacks are set to NULL. Memory and Stack are
// initialized. The datapath is reset. Input and display are cleared.
//
CHEMU_API ChipEmu chipemu_create(void);

//
// Signals the emulation thread to resume execution. The emulator must be in
// the IDLE state when calling this function, otherwise this function does
// nothing. If successful, the emulator will be in the LOOP state and the
// emulator thread will continue looping.
//
CHEMU_API ChipEmuState chipemu_continue(ChipEmu emu);

//
// Frees all resources used by the given emulator handle.
//
CHEMU_API void chipemu_destroy(ChipEmu emu);

//
// Executes the given instruction without modifying the program counter. If
// the given instruction fails to execute or is illegal, the emulator state is
// set to ERROR.
//
CHEMU_API ChipInstResult chipemu_execute(ChipEmu emu, ChipInst inst);

//
// Invokes the GetKeyCallback associated with the emulator. If no callback is
// associated with the emulator then the function will return CHIP_KEY_0.
// This function is used only by cif_lk and is not part of the API
//
ChipKey chipemu_getKey(ChipEmu emu);

//
// Stores a copy of the display into the given ChipDisplay pointer.
//
CHEMU_API void chipemu_getDisplay(ChipEmu emu, ChipDisplay *displayDest);

//
// Stores a copy of the stack into the given ChipStack pointer.
//
CHEMU_API void chipemu_getStack(ChipEmu emu, ChipStack *stackDest);

//
// Stores a copy of the datapath (program counter, registers, timers) into the
// given ChipDP pointer.
//
CHEMU_API void chipemu_getDatapath(ChipEmu emu, ChipDP *datapathDest);

//
// Loads a CHIP-8 ROM file into the emulator's memory in the data section.
// The size of the ROM in bytes is returned on success. If any read error
// occurs or the ROM file was too big, -1 is returned and the emulator
// is unmodified.
//
CHEMU_API int chipemu_loadROM(ChipEmu emu, const char *path);

//
// Begins execution of the main loop in the calling thread. This function will
// loop and call chipemu_step repeatedly until an error occurs or if signaled
// by chipemu_stop. This function returns zero if signaled to stop and nonzero
// if an error occured.
//
CHEMU_API int chipemu_mainLoop(ChipEmu emu);

//
// Updates the state of a key in the emulator. This function may wake up the
// emulator thread if it is waiting on a key press
//
CHEMU_API void chipemu_setKey(ChipEmu emu, ChipKey key, ChipKeyState state);

//
// Starts execution of the main loop in a separate thread. The emulator must be
// in the READY state when calling this function, otherwise nothing happens.
// The emulator's state is set to LOOP on success.
//
CHEMU_API ChipEmuState chipemu_start(ChipEmu emu);

//
// Stops execution of the main loop in the emulator thread. The emulator must
// be in the LOOP or IDLE state when calling this function. The emulator's
// state is set to READY on success.
//
CHEMU_API ChipEmuState chipemu_stop(ChipEmu emu);

//
// Invokes the RedrawCallback associated with the given emulator. If no
// callback is associated with the emulator, the function does nothing. This
// function is called by the draw and cls instructions and is not part of the
// API.
//
void chipemu_redraw(ChipEmu emu);

//
// Resets the state of the emulator to the default state.
//
// The default state is the following:
//   1. The program counter is set to 0x200
//   2. All registers and timers are set to 0
//   3. The stack is cleared
//   4. The input is cleared
//   5. The display is cleared
//
// Note that reset does NOT clear the data section of the emulator's memory,
// chipemu_clear should be called for that purpose.
//
// The emulator state is set to READY, and if necessary, all associated threads
// are stopped. This function will always return CHIPEMU_READY
//
CHEMU_API ChipEmuState chipemu_reset(ChipEmu emu);

//
// Emulates a single cycle for the given emulator object. The emulator must be
// in the READY or IDLE state when calling this function.
//
CHEMU_API int chipemu_step(ChipEmu emu);

#endif

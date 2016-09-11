#ifndef _EMULATION_H
#define _EMULATION_H

#include "ChipEmu.h"
#include "internal/export.h"

#define CHIP_STEP_SUCCESS 0
#define CHIP_STEP_FAILURE 1

#define CHIP_LOAD_SUCCESS 0
#define CHIP_LOAD_FAILURE 1

#define chipemu_create() (ChipEmu*)malloc(sizeof(ChipEmu));
#define chipemu_destroy(emu) free(emu);
#define chipemu_stop(emu) emu->running = false;

//
// Initializes the emulator. Callbacks are set to NULL. Memory and Stack are
// initialized. The datapath is reset. Input and display are cleared.
//
CHEMU_API void chipemu_init(ChipEmu *emu);

//
// Invokes the pollKeyCallback associated with the emulator. If no callback is
// associated with the emulator, the function does nothing except return
// CHIP_KEY_0
//
CHEMU_API ChipKey chipemu_getKey(ChipEmu *emu);

//
// Loads a CHIP-8 ROM file into the emulator's memory in the data section.
// The size of the ROM in bytes is returned on success. If any read error
// occurs or the ROM file was too big, -1 is returned and the emulator
// is unmodified.
//
CHEMU_API int chipemu_loadROM(ChipEmu *emu, const char *path);

//
// Starts the main loop
CHEMU_API int chipemu_mainLoop(ChipEmu *emu);

CHEMU_API void chipemu_setKey(ChipEmu *emu, ChipKey key, ChipKeyState state);

//
// Invokes the redrawCallback associated with the given emulator. If no
// callback is associated with the emulator, the function does nothing.
//
CHEMU_API void chipemu_redraw(ChipEmu *emu);

//
// Resets the state of the emulator to the default state. If the emulator
// is running, this function does nothing.
//
// The default state is the following:
//   1. The program counter is set to 0x200
//   2. All registers and timers are set to 0
//   3. The stack is cleared
//   4. The input is cleared
//   5. The display is cleared
//
CHEMU_API void chipemu_reset(ChipEmu *emu);

//
// Emulates a single cycle for the given emulator object.
//
CHEMU_API int chipemu_step(ChipEmu *emu);

#endif

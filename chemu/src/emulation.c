#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "emulation.h"
#include "instructions.h"
#include "memory.h"
#include "stack.h"
#include "debug.h"



ChipEmu* chipemu_create() {
    ChipEmu *emu = (ChipEmu*)malloc(sizeof(ChipEmu));
    emu->dp = CHIPDP_INIT;
    emu->dp.pc = CHIP_PRGM_START;

    chipmem_init(&emu->memory);

    chipstack_init(&emu->stack);

    return emu;
}

int chipemu_mainLoop(ChipEmu *emu) {

    int exitStatus = EXIT_SUCCESS;

    for (;;) {

        // fetch
        ChipInst instruction = {
            .instruction = 0
        };

    }

    return exitStatus;
}

ChipInstFunc chipemu_decode(ChipInst inst) {
    return NULL;
}

void chipemu_destroy(ChipEmu *emu) {
    free(emu);
}

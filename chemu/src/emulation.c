#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "chemu/emulation.h"
#include "chemu/decode.h"
#include "chemu/display.h"
#include "chemu/memory.h"
#include "chemu/stack.h"
#include "debug.h"


ChipEmu* chipemu_create() {
    ChipEmu *emu = (ChipEmu*)malloc(sizeof(ChipEmu));

    emu->pollKeyHandler = NULL;
    emu->pollInputHandler = NULL;

    chipmem_init(&emu->memory);

    chipstack_init(&emu->memory.reserved.stack);

    return emu;
}

void chipemu_init(ChipEmu *emu) {
    // set callbacks to NULL
    emu->pollKeyHandler = NULL;
    emu->pollInputHandler = NULL;
    emu->drawCallback = NULL;

    chipmem_init(&emu->memory);
    chipstack_init(&emu->memory.reserved.stack);

    // clear datapath
    emu->memory.reserved.pc = CHIP_PRGM_START;
    emu->memory.reserved.addrReg = 0;
    for (int i = 0; i < 16; ++i)
        emu->memory.reserved.regs[i] = 0;
    emu->memory.reserved.sndTimer = 0;
    emu->memory.reserved.delTimer = 0;

    // clear input
    emu->memory.reserved.input = 0;

    // clear framebuffer
    chipdisplay_clear(&emu->memory.reserved.display);
}

int chipemu_loadROM(ChipEmu *emu, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp != NULL) {
        fread(emu->memory.array + CHIP_PRGM_START, 1, CHIPMEM_DATA_LEN, fp);
        fclose(fp);
        return CHIP_LOAD_SUCCESS;
    }
    return CHIP_LOAD_FAILURE;
}

int chipemu_mainLoop(ChipEmu *emu) {

    int exitStatus = EXIT_SUCCESS;

    for (;;) {

        // emulate cycle
        chipemu_step(emu);

        // poll inputs if a handler has been assigned
        // if (emu->pollInputHandler != NULL)
        //     emu->pollInputHandler(&emu->input);


    }

    return exitStatus;
}


// void chipemu_reset(ChipEmu *emu) {
//     // clear datapath
//     emu->memory.reserved.pc = CHIP_PRGM_START;
//     emu->memory.reserved.addrReg = 0;
//     for (int i = 0; i < 16; ++i)
//         emu->memory.reserved.regs[i] = 0;
//     emu->memory.reserved.sndTimer = 0;
//     emu->memory.reserved.delTimer = 0;
//
//     // clear input
//     emu->memory.reserved.input = 0;
//
//     // clear framebuffer
//     chipdisplay_clear(&emu->memory.reserved.display);
//
//     // memory is left as is, must be cleared manually
//
//     // clear stack
//     chipstack_init(&emu->memory.reserved.stack);
// }

int chipemu_step(ChipEmu *emu) {
    int result = CHIP_STEP_SUCCESS;

    // fetch
    ChipAddress *pc = &emu->memory.reserved.pc;
    ChipInst instruction = (uint16_t)(emu->memory.array[*pc] << 8 |
                                      emu->memory.array[*pc + 1]);


    // decode
    int i = chipdec_index(instruction);
    if (i == NO_INSTRUCTION)
        return CHIP_STEP_FAILURE;

    ChipOp op = CHIP_OPTABLE[i];
    ChipInstDec decoded = chipdec_decode(instruction, op.cls);

    // execute
    ChipInstResult instResult = op.func(emu, decoded);
    switch (instResult) {
        case INST_SUCCESS:
            break;
        case INST_SUCCESS_INCR_PC:
            *pc += 2;
            break;
        case INST_FAILURE:
            result = CHIP_STEP_FAILURE;
            break;
    }

    return result;
}

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

int chipemu_loadROM(ChipEmu *emu, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp != NULL) {
        fread(emu->memory.data, 1, CHIPMEM_DATA_LEN, fp);
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
        if (emu->pollInputHandler != NULL)
            emu->pollInputHandler(&emu->input);


    }

    return exitStatus;
}

void chipemu_destroy(ChipEmu *emu) {
    free(emu);
}

void chipemu_reset(ChipEmu *emu) {
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

    // memory is left as is, must be cleared manually

    // clear stack
    chipstack_init(&emu->memory.reserved.stack);
}

int chipemu_step(ChipEmu *emu) {
    int result = CHIP_STEP_SUCCESS;

    // fetch
    ChipInst instruction = {
        .instruction = (uint16_t)(emu->memory.array[emu->dp.pc] << 8 |
                                  emu->memory.array[emu->dp.pc + 1])
    };


    // decode
    ChipInstFunc func = chipemu_decode(instruction);

    // execute
    int instResult = (*func)(emu, instruction);
    switch (instResult) {
        case INST_SUCCESS:
            break;
        case INST_SUCCESS_INCR_PC:
            emu->dp.pc += 2;
            break;
        case INST_FAILURE:
            result = CHIP_STEP_FAILURE;
            break;
    }

    return result;
}

static ChipInstResult cif_cat0(ChipEmu *emu, ChipInst inst) {
    switch (inst.instruction) {
        case 0x00E0:
            return cif_cls(emu, inst);
            break;
        case 0x00EE:
            return cif_ret(emu, inst);
            break;
        default:
            return cif_sys(emu, inst);
            break;
    }
}

static ChipInstResult cif_cat8(ChipEmu *emu, ChipInst inst) {
    int nibble = inst.instruction & 0xF;
    switch (nibble) {
        case 0:
            return cif_move(emu, inst);
            break;
        case 1:
            return cif_or(emu, inst);
        case 2:
            return cif_and(emu, inst);
        case 3:
            return cif_xor(emu, inst);
        case 4:
            return cif_add(emu, inst);
        case 5:
            return cif_sub(emu, inst);
        case 6:
            return cif_shr(emu, inst);
        case 7:
            return cif_subn(emu, inst);
        default:
            if (nibble == 0xE)
                return cif_shl(emu, inst);
            else
                return INST_ILLEGAL;
    }

}

static ChipInstResult cif_cate(ChipEmu *emu, ChipInst inst) {
    switch (inst.instruction & 0xFF) {
        case 0x9E:
            return cif_sip(emu, inst);
        case 0xA1:
            return cif_snip(emu, inst);
        default:
            return INST_ILLEGAL;
    }
}

static ChipInstResult cif_catf(ChipEmu *emu, ChipInst inst) {

}

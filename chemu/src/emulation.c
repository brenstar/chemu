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

    emu->dp.pc = CHIP_PRGM_START;
    emu->pollKeyHandler = NULL;
    emu->pollInputHandler = NULL;

    chipmem_init(&emu->memory);

    chipstack_init(&emu->stack);

    return emu;
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

ChipInstFunc chipemu_decode(ChipInst inst) {
    ChipInstFunc func = NULL;

    uint16_t instruction = inst.instruction;
    // macro for assigning the func pointer by mnemonic name of instruction
    // ex: assignByMnemonic(foo)
    //    --> debug(...); func = &cif_foo;
    #define assignByMnemonic(name) debug("Decoded 0x%04x as " #name "\n", instruction); \
                                   func = &cif_ ## name


    switch ((instruction & 0xF000) >> 12) {
        case 0x0:
            switch (instruction) {
                case 0x00E0:
                    assignByMnemonic(cls);
                    break;
                case 0x00EE:
                    assignByMnemonic(ret);
                    break;
                default:
                    assignByMnemonic(sys);
                    break;
            }
            break;
        case 0x1:
            assignByMnemonic(j);
            break;
        case 0x2:
            assignByMnemonic(call);
            break;
        case 0x3:
            assignByMnemonic(sei);
            break;
        case 0x4:
            assignByMnemonic(sni);
            break;
        case 0x5:
            if ((instruction & 0xF) == 0) {
                assignByMnemonic(se);
            }
            break;
        case 0x6:
            assignByMnemonic(li);
            break;
        case 0x7:
            assignByMnemonic(addi);
            break;
        case 0x8:
            switch (instruction & 0xF) {
                case 0x0:
                    assignByMnemonic(move);
                    break;
                case 0x1:
                    assignByMnemonic(or);
                    break;
                case 0x2:
                    assignByMnemonic(and);
                    break;
                case 0x3:
                    assignByMnemonic(xor);
                    break;
                case 0x4:
                    assignByMnemonic(add);
                    break;
                case 0x5:
                    assignByMnemonic(sub);
                    break;
                case 0x6:
                    assignByMnemonic(shr);
                    break;
                case 0x7:
                    assignByMnemonic(subn);
                    break;
                case 0xE:
                    assignByMnemonic(shl);
                    break;
                default:
                    break;
            }
            break;
        case 0x9:
            if ((instruction & 0xF) == 0x0) {
                assignByMnemonic(sn);
            }
            break;
        case 0xA:
            assignByMnemonic(la);
            break;
        case 0xB:
            assignByMnemonic(jo);
            break;
        case 0xC:
            assignByMnemonic(rnd);
            break;
        case 0xD:
            assignByMnemonic(draw);
            break;
        case 0xE:
            switch (instruction & 0xFF) {
                case 0x9E:
                    assignByMnemonic(sip);
                    break;
                case 0xA1:
                    assignByMnemonic(snip);
                    break;
                default:
                    break;
            }
            break;
        case 0xF:
            switch (instruction & 0xFF) {
                case 0x07:
                    assignByMnemonic(ld);
                    break;
                case 0x0A:
                    assignByMnemonic(lk);
                    break;
                case 0x15:
                    assignByMnemonic(del);
                    break;
                case 0x18:
                    assignByMnemonic(snd);
                    break;
                case 0x1E:
                    assignByMnemonic(ii);
                    break;
                case 0x29:
                    assignByMnemonic(font);
                    break;
                case 0x33:
                    assignByMnemonic(bcd);
                    break;
                case 0x55:
                    assignByMnemonic(save);
                    break;
                case 0x65:
                    assignByMnemonic(rest);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return func;
}

void chipemu_destroy(ChipEmu *emu) {
    free(emu);
}

void chipemu_reset(ChipEmu *emu) {
    // clear datapath
    emu->dp.pc = CHIP_PRGM_START;
    emu->dp.addrReg = 0;
    for (int i = 0; i < 16; ++i)
        emu->dp.regs[i] = 0;
    emu->dp.sndTimer = 0;
    emu->dp.delTimer = 0;

    // memory is left as is, must be cleared manually

    // clear stack
    chipstack_init(&emu->stack);
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

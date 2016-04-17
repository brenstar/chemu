
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "chipemu.h"
#include "debug.h"
#include "instructions.h"





// reserved memory locations

#define CHIP_FONTSET_START 0x0


// digit sprites representing digits 0-F as 8x5 sprites.
static const uint8_t DIGITS[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};






ChipEmu chipemu_create() {
    ChipEmu emu = (ChipEmu)malloc(sizeof(struct ChipEmuStruct));
	emu->pc = CHIP_PRGM_START;

    //emu->sp = &(emu->stack);
    emu->addressReg = 0;
    emu->sndTimer = 0;
    emu->delayTimer = 0;

    for (int i = 0; i < REGISTERS; ++i)
        emu->registers[i] = 0;

    memcpy(emu->memory + CHIP_FONTSET_START, DIGITS, sizeof(DIGITS));

    emu->memory[CHIP_PRGM_START] = 0xFF;
    emu->memory[CHIP_PRGM_START + 1] = 0xFF;

    emu->stack = (ChipStack)malloc(sizeof(struct ChipStackStruct));
    emu->stack->index = 0;

    // seed the random
    srand(time(NULL));

    return emu;
}

int chipemu_mainLoop(ChipEmu emu) {

    int exitStatus = EXIT_SUCCESS;

    for (;;) {

        // fetch
        uint16_t instruction = emu->memory[emu->pc] << 8 | emu->memory[emu->pc + 1];
        if (instruction == ILLEGAL_INST) {
            logInfo("finished execution\n");
            break;
        }

        // decode
        //int (*instFunc)(ChipEmu, uint16_t);
        ChipInstFunc instFunc;

        // instFunc will be null if decoding fails
        instFunc = chipemu_decode(instruction);

        // execute
        if (instFunc) {
            bool execFailure = false;
            int result = (*instFunc)(emu, instruction);

            switch (result) {
                case INST_FAILURE:
                    execFailure = true;
                    break; // exit loop, instruction failed to execute
                case INST_SUCCESS_INCR_PC:
                    emu->pc += 2; // increment pc, instruction successful
                    break;
                default:
                    break;
            }

            if (execFailure) {
                exitStatus = EXIT_FAILURE;
                break; // end emulation, instruction failed to execute
            }

        } else {
            logError("Illegal instruction 0x%04x at %d\n", instruction, emu->pc);
            exitStatus = EXIT_FAILURE;
            break;
        }

    }

    return exitStatus;
}

ChipInstFunc chipemu_decode(uint16_t instruction) {
    ChipInstFunc func = NULL;

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

void chipemu_destroy(ChipEmu emu) {
    //chipmem_destroy(emu->memory);
    free(emu->stack);
    free(emu);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chemu/emulation.h"
#include "chemu/instructions.h"
#include "chemu/decode.h"
#include "chemu/logger.h"

static void dumpRegisters(ChipDP *dp);

int main(int argc, const char* argv[]) {

    puts("Enter an instruction in hexadecimal to execute");

    chiplog_set(stderr);
    chiplog_setLevel(CHIP_LOG_DEBUG);

    //ChipEmu *emu = (ChipEmu*)malloc(sizeof(ChipEmu));
    //chipemu_init(emu);
    ChipEmu emu = chipemu_create();
    ChipInstResult lastResult = INST_SUCCESS;
    ChipDP dp;
    chipemu_getDatapath(emu, &dp);

    while (!feof(stdin)) {

        printf("[PC: 0x%03X][%d]>>> ", dp.pc, lastResult);

        int scannedInst;
        int charsRead = scanf("%4x", &scannedInst);

        if (charsRead == 4) {
            ChipInst inst = (ChipInst)scannedInst;
            lastResult = chipemu_execute(emu, inst);
            chipemu_getDatapath(emu, &dp);
            dumpRegisters(&dp);
        }

        // printf("[PC: 0x%03X][%d]>>> ", emu->memory.reserved.pc, lastResult);
        // char *line = NULL;
        // size_t bufsize, size;
        // if ((size = getline(&line, &bufsize, stdin)) != -1) {
        //     if (size - 1 == 4) {
        //         //uint16_t inst = (uint16_t)strtol(line, NULL, 16);
        //         ChipInst inst = (ChipInst)strtol(line, NULL, 16);
        //
        //         int index = chipdec_index(inst);
        //         if (index != NO_INSTRUCTION) {
        //             ChipOp op = CHIP_OPTABLE[index];
        //             ChipInstDec decoded = chipdec_decode(inst, op.cls);
        //             lastResult = op.func(emu, decoded);
        //             dumpRegisters(emu);
        //         } else
        //             printf("Illegal instruction\n");
        //
        //
        //
        //     }
        // }

    }

    chipemu_destroy(emu);

    return 0;
}

static void dumpRegisters(ChipDP *dp) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int reg = (i * 4) + j;
            printf("v%-2d: %3d        ", reg, dp->regs[reg]);
        }
        putchar('\n');
    }

    printf("I: %03x\t", dp->addrReg);
    printf("st: %d\t", dp->sndTimer);
    printf("dt: %d\n", dp->delTimer);
}

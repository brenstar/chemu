#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chemu/emulation.h"
#include "chemu/instructions.h"
#include "chemu/decode.h"

#define RESERVED(emu) emu->memory.reserved

static void dumpRegisters(ChipEmu *emu);

int main(int argc, const char* argv[]) {

    puts("Enter an instruction in hexadecimal to execute");

    ChipEmu *emu = (ChipEmu*)malloc(sizeof(ChipEmu));
    ChipInstResult lastResult = INST_SUCCESS;

    while (!feof(stdin)) {

        printf("[%d]>>> ", lastResult);
        char *line = NULL;
        size_t bufsize, size;
        if ((size = getline(&line, &bufsize, stdin)) != -1) {
            if (size - 1 == 4) {
                //uint16_t inst = (uint16_t)strtol(line, NULL, 16);
                ChipInst inst = (ChipInst)strtol(line, NULL, 16);

                int index = chipdec_index(inst);
                if (index != NO_INSTRUCTION) {
                    ChipOp op = CHIP_OPTABLE[index];
                    ChipInstDec decoded = chipdec_decode(inst, op.cls);
                    lastResult = op.func(emu, decoded);
                    dumpRegisters(emu);
                } else
                    printf("Illegal instruction\n");



            }
        }

    }

    /*
    if (argc == 2) {
        uint16_t insts[4096];

        FILE* rom = fopen(argv[1], "r");

        size_t instructionCount = fread(insts, sizeof(uint16_t), 2048, rom);

        fclose(rom);

        for (size_t i = 0; i < instructionCount; ++i) {
            printf("%d: Decoding instruction 0x%04x\n", i, insts[i]);
            ChipInstFunc func = chipemu_decode(insts[i]);
            if (func != NULL)
                printf("Function for instruction: %p\n", func);
            printf("\n");
        }
    }*/
    //
    // for (int i = 0; i < sizeof(insts) / sizeof(insts[0]); ++i) {
    //     printf("%d: Decoding instruction 0x%04x\n", i, insts[i]);
    //     ChipInstFunc func = chipemu_decode(insts[i]);
    //     if (func != NULL)
    //         printf("Function for instruction: %p\n", func);
    //     printf("\n");
    // }

    free(emu);

    return 0;
}

static void dumpRegisters(ChipEmu *emu) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int reg = (i * 4) + j;
            printf("v%-2d: %3d        ", reg, RESERVED(emu).regs[reg]);
        }
        putchar('\n');
    }

    printf("I: %03x\t", RESERVED(emu).addrReg);
    printf("st: %d\t", RESERVED(emu).sndTimer);
    printf("dt: %d\n", RESERVED(emu).delTimer);
}

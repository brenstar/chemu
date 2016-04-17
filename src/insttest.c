#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chipemu.h"
#include "instructions.h"

static void dumpRegisters(ChipEmu emu);

int main(int argc, const char* argv[]) {

    ChipEmu emu = chipemu_create();

    while (!feof(stdin)) {

        printf(">>> ");
        char *line = NULL;
        size_t bufsize, size;
        if ((size = getline(&line, &bufsize, stdin)) != -1) {
            if (size - 1 == 4) {
                uint16_t inst = (uint16_t)strtol(line, NULL, 16);
                ChipInstFunc func = chipemu_decode(inst);
                if (func != NULL) {
                    (*func)(emu, inst);
                    dumpRegisters(emu);
                }
                else
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

    chipemu_destroy(emu);

    return 0;
}

static void dumpRegisters(ChipEmu emu) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int reg = (i * 4) + j;
            printf("v%-2d: %3d        ", reg, emu->registers[reg]);
        }
        putchar('\n');
    }

    printf("I: %03x\t", emu->addressReg);
    printf("st: %d\t", emu->sndTimer);
    printf("dt: %d\n", emu->delayTimer);
}


#include <stdio.h>
#include <stdlib.h>

#include "chipemu.h"

int main(int argc, const char *argv[]) {

    if (argc != 2) {
        printf("usage: chemu <file>\n");
        return EXIT_FAILURE;
    }

    ChipEmu emu = chipemu_create();

    int result = chipemu_mainLoop(emu);

    chipemu_destroy(emu);

    return result;
}

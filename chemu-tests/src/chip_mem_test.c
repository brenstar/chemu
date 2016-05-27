#include <assert.h>
#include <stdio.h>

#include "memory.h"


int main() {

    ChipMem *mem = chipmem_create();

    // print the entire fontset, with each digit (5 bytes) on one line
    uint8_t *fontsetPtr = mem->reserved.fontset;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 5; ++j)
            printf("%02x ", *(fontsetPtr++));
        printf("\n");
    }

    chipmem_destroy(mem);


    return 0;
}


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "chemu/memory.h"

#define boolstr(x) x ? "true" : "false"

typedef struct {
    ChipAddress addr;
    uint8_t value;
    uint8_t expectedRead;
    bool expectedWrite;
} TestCase;

static const TestCase TESTS[] = {
    {0x200, 0xFF, 0xFF, true},
    {0xFFF, 0x54, 0x54, true},
    {0x1FF, 0xF0, 0x0, false}
};

int main() {

    printf("Size of ChipMem: %d (expected: %d)\n", sizeof(ChipMem), CHIPMEM_LEN);
    assert(sizeof(ChipMem) == CHIPMEM_LEN);

    printf("Size of ChipMem_reserved: %d (must be <= 512)\n\n", sizeof(ChipMem_reserved));
    assert(sizeof(ChipMem_reserved) <= CHIPMEM_RESERVED_LEN);

    ChipMem *mem = (ChipMem*)malloc(sizeof(ChipMem));

    chipmem_init(mem);

    size_t fontsetOffset = offsetof(ChipMem_reserved, fontset);
    printf("Checking fontset\n");
    for (int i = fontsetOffset; i < CHIPMEM_FONTSET_LEN; ++i)
        assert(mem->array[i] == FONTSET[i]);


    printf("Fontset is located at 0x%03X\n", fontsetOffset);
    for (uint8_t i = 0; i < 16; ++i) {
        ChipAddress addr = chipmem_get_font(i);
        printf("Digit %X is located at 0x%03X\n", i, addr);
    }

    printf("Checking memory contents\n");
    for (int i = sizeof(ChipMem_reserved); i < CHIP_END; ++i)
        assert(mem->array[i] == 0);

    for (size_t i = 0; i < 3; ++i) {
        TestCase test = TESTS[i];
        printf("Writing 0x%02X at location 0x%03X...", test.value, test.addr);
        bool result = chipmem_write(mem, test.addr, test.value);
        printf("%s (expected: %s)\n", boolstr(result), boolstr(test.expectedWrite));
    }

    for (size_t i = 0; i < 3; ++i) {
        TestCase test = TESTS[i];
        printf("Reading from 0x%03X...", test.addr);
        uint8_t result = chipmem_read(mem, test.addr);
        printf("%02X (expected: %02X)\n", result, test.expectedRead);
    }

    free(mem);

    return 0;
}

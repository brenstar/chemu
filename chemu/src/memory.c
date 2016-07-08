#include "chemu/memory.h"

#include <string.h>

const uint8_t FONTSET[] = {
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

void chipmem_init(ChipMem *mem) {
    memcpy(mem->reserved.fontset, FONTSET, sizeof(FONTSET) / sizeof(uint8_t));
    memset(mem->array + CHIPMEM_FONTSET_LEN, 0, CHIP_END - CHIPMEM_FONTSET_LEN);
}

inline uint8_t chipmem_read(ChipMem *mem, ChipAddress addr) {
    return (addr <= CHIP_END) ? mem->array[addr] : 0;
}

inline bool chipmem_write(ChipMem *mem, ChipAddress addr, uint8_t value) {
    // check if the address is not in the reserved portion of memory
    if (addr >= CHIP_PRGM_START) {
        mem->array[addr] = value;
        return true;
    }
    return false;
}


ChipAddress chipmem_get_font(ChipMem *mem, uint8_t digit) {
    //return mem->reserved + CHIP_FONTSET_START + (digit * 5);
    return CHIPMEM_FONTSET_START + digit * 5;
}

#include "memory.h"

INLINE uint8_t chipmem_read(ChipMem *mem, ChipAddress addr) {
    return (addr <= CHIP_END) ? mem->array[addr] : 0;
}

INLINE bool chipmem_write(ChipMem *mem, ChipAddress addr, uint8_t value) {
    // check if the address is not in the reserved portion of memory
    if (addr >= CHIP_PRGM_START) {
        mem->array[addr] = value;
        return true;
    }
    return false;
}
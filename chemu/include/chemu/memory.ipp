#include "memory.h"

#include "logger.h"

INLINE uint8_t chemu_mem_read(ChipMem *mem, ChipAddress addr) {
    return (addr <= CHIP_END) ? mem->array[addr] : 0;
}

INLINE bool chemu_mem_write(ChipMem *mem, ChipAddress addr, uint8_t value) {
    // check if the address is not in the reserved portion of memory
    if (addr >= CHIP_PRGM_START) {
        mem->array[addr] = value;
        return true;
    }
	chiplog_warn("Attempted write to reserved address 0x%03X\n", addr);
    return false;
}
#ifndef _CHIP_MEM_H
#define _CHIP_MEM_H

#include "ChipMem.h"
#include "ChipAddress.h"
#include "internal/export.h"

#include <stdbool.h>

//
// Macro determines if a ChipAddress is valid (0 <= x <= 0xFFF)
//
#define checkAddr(addr) (addr >= CHIP_PRGM_START && addr <= CHIP_END)

//
// Constant definition containing the fontset. The fontset contains 16 sprites,
// numbers 0-9 and letters A-F. Each sprite is 5 bytes long.
//
CHEMU_API extern const uint8_t FONTSET[CHIPMEM_FONTSET_LEN];

//
// Initializes the ChipMem object. The fontset is copied into the reserved
// section at location 0x0 and the rest of the array is set to 0
//
CHEMU_API void chipmem_init(ChipMem *mem);

//
// Reads a byte at the given address from the given ChipMem object. Valid
// addresses range from 0 to 4095. The caller is responsible for checking the
// bounds of the addr parameter. If addr is greater than 4095, 0 is returned.
//
CHEMU_API uint8_t chipmem_read(ChipMem *mem, ChipAddress addr);

//
// Writes a given byte at the given address in the given ChipMem object. true
// is returned on sucessful write, false otherwise. False is returned if the
// addr parameter is between 0 and CHIPMEM_RESERVED_LEN (reserved portion) and
// the byte will not be written.
//
CHEMU_API bool chipmem_write(ChipMem *mem, ChipAddress addr, uint8_t value);

//
// Gets the address of the font sprite for the given digit. The digit
// parameter must be in the range of 0 to 15.
//
CHEMU_API ChipAddress chipmem_get_font(uint8_t digit);

#if defined(_INLINE_) && defined(_MSC_VER)
#include "internal/inline.h"
#include "memory.ipp"
#endif

#endif

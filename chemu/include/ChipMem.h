#ifndef _CHIPMEM_H
#define _CHIPMEM_H

#include <stdint.h>

#define CHIPMEM_RESERVED_LEN 512   // length in bytes of reserved memory
#define CHIPMEM_DATA_LEN 3584      // length in bytes of data memory

// total length (bytes) of memory used by CHIP-8
#define CHIPMEM_LEN CHIPMEM_RESERVED_LEN + CHIPMEM_DATA_LEN

// fontset: 16 sprites, each 5 bytes long, 80 total
#define CHIPMEM_FONTSET_LEN 80    // length (bytes) of the fontset

// data memory locations

#define CHIP_PRGM_START 0x200
#define CHIP_ETIPRGM_START 0x600
#define CHIP_END 0xFFF

// only the fontset is stored in the reserved section at offset 0
/*
 * Type representing the reserved portion of CHIP-8 memory, as a union.
 */
typedef union ChipMem_reserved_u {
    uint8_t array[CHIPMEM_RESERVED_LEN];
    uint8_t fontset[CHIPMEM_FONTSET_LEN];
} ChipMem_reserved;

typedef uint8_t ChipMem_data[CHIPMEM_DATA_LEN];


typedef struct ChipMem_s {
    ChipMem_reserved reserved;
    ChipMem_data data;
} ChipMem;


#endif

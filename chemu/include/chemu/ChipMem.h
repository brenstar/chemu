#ifndef _CHIPMEM_H
#define _CHIPMEM_H

#include <stdint.h>

#include "ChipDisplay.h"
#include "ChipStack.h"
#include "ChipAddress.h"
#include "ChipReg.h"
#include "ChipInput.h"


// total length (bytes) of memory used by CHIP-8
#define CHIPMEM_LEN 4096

#define CHIPMEM_RESERVED_LEN 512

#define CHIPMEM_DATA_LEN CHIPMEM_LEN - CHIPMEM_RESERVED_LEN

// fontset: 16 sprites, each 5 bytes long, 80 total
#define CHIPMEM_FONTSET_LEN 80    // length (bytes) of the fontset

// data memory locations

#define CHIP_PRGM_START 0x200
#define CHIP_ETIPRGM_START 0x600
#define CHIP_END 0xFFF

typedef struct ChipMem_reserved_s {
    ChipDisplay display;                    // 0    (256 bytes)
    ChipAddress pc;                         // 256  (2 bytes)
    ChipAddress addrReg;                    // 258  (2 bytes)
    ChipInput input;                        // 260  (2 bytes)
    ChipStack stack;                        // 262  (34 bytes)
    ChipReg regs[16];                       // 296  (16 bytes)
    ChipReg sndTimer;                       // 312  (1 byte)
    ChipReg delTimer;                       // 313  (1 byte)
    uint8_t fontset[CHIPMEM_FONTSET_LEN];   // 315  (80 bytes)
} ChipMem_reserved;                         // Total: 394, with 118 bytes left for padding

typedef union ChipMem_u {
    uint8_t array[CHIPMEM_LEN];
    ChipMem_reserved reserved;
} ChipMem;

#endif

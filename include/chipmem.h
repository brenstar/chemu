#ifndef _CHIPMEM_H
#define _CHIPMEM_H

#include <stdint.h>

#define CHIPMEM_RESERVED_LEN 512
#define CHIPMEM_DATA_LEN 3584

// data memory locations

#define CHIP_PRGM_START 0x200
#define CHIP_ETIPRGM_START 0x600
#define CHIP_END 0xFFF

// reserved memory locations

#define CHIP_FONTSET_START 0x0
#define CHIP_PC 0x50
#define CHIP_SP 0x52
#define CHIP_STACK 0x54
#define CHIP_REGISTERS 0x74
#define CHIP_SND_TIMER 0x84
#define CHIP_DEL_TIMER 0x85
#define CHIP_ADDR_REGISTER 0x86

struct ChipMemStruct {
    uint8_t reserved[CHIP_PRGM_START];
    uint8_t data[CHIP_END - CHIP_PRGM_START + 1];
};

typedef struct ChipMemStruct * ChipMem;



ChipMem chipmem_create();

void chipmem_init(ChipMem mem);

uint8_t* chipmem_get_font(ChipMem mem, char digit);

void chipmem_destroy(ChipMem mem);

#endif

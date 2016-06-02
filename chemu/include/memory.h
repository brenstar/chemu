#ifndef _CHIP_MEM_H
#define _CHIP_MEM_H

#include "ChipMem.h"


ChipMem* chipmem_create();

void chipmem_init(ChipMem *mem);

uint16_t chipmem_get_font(ChipMem *mem, uint8_t digit);

void chipmem_destroy(ChipMem *mem);

#endif

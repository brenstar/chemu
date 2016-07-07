#ifndef _CHIP_MEM_H
#define _CHIP_MEM_H

#include "ChipMem.h"
#include "ChipDP.h"
#include "ChipAddress.h"

//ChipMem* chipmem_create();

extern const uint8_t FONTSET[];

void chipmem_init(ChipMem *mem);

uint8_t chipmem_read(ChipMem *mem, ChipAddress addr);

bool chipmem_write(ChipMem *mem, ChipAddress addr, uint8_t value);

ChipAddress chipmem_get_font(ChipMem *mem, uint8_t digit);


//void chipmem_destroy(ChipMem *mem);

#endif

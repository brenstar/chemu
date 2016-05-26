#ifndef _CHIPMEM_H
#define _CHIPMEM_H

#include <stdint.h>
#include "chipstack.h"

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



typedef uint8_t[80] ChipMem_fontset;

// CHIP-8 datapath struct
typedef struct ChipMem_dp_s {
  uint16_t pc;
  uint16_t addrReg;
  uint8_t regs[16];
  uint8_t sndTimer;
  uint8_t delTimer;
} ChipMem_dp;



typedef union ChipMem_u {

	uint8_t asArray[CHIPMEM_RESERVED_LEN + CHIPMEM_DATA_LEN];

	struct {
        union {
            uint8_t reservedArray[CHIPMEM_RESERVED_LEN];
            struct {
                ChipMem_fontset fontset;
                ChipMem_dp dp;
                ChipStack stack;
            } reserved;
        }
    };
    uint8_t data[CHIPMEM_DATA_LEN];
 };

} * ChipMem;


ChipMem chipmem_create();

void chipmem_init(ChipMem mem);

uint8_t* chipmem_get_font(ChipMem mem, char digit);

void chipmem_destroy(ChipMem mem);

#endif

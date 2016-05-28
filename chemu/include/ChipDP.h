#ifndef _CHIPDP_H
#define _CHIPDP_H

#include <stdint.h>

// CHIP-8 Datapath structure
typedef struct ChipDP_s {
    uint16_t pc;        // program counter
    uint16_t addrReg;   // address Register I
    uint8_t regs[16];   // data registers v0 - vF
    uint8_t sndTimer;   // sound timer
    uint8_t delTimer;   // delay timer
} ChipDP;


#endif

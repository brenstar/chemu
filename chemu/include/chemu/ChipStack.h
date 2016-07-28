#ifndef _CHIPSTACK_H
#define _CHIPSTACK_H

#include <stdint.h>
#include "ChipAddress.h"

#define CHIP_STACK_SIZE 16


// typedef struct {
//     ChipAddress *sp;  // stack pointer
// 	//uint8_t index;
// 	ChipAddress data[CHIP_STACK_SIZE];
// } ChipStack;

typedef struct ChipStack_s {
    uint8_t index;
    ChipAddress data[CHIP_STACK_SIZE];
} ChipStack;

#endif

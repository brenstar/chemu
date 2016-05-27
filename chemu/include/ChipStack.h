#ifndef _CHIPSTACK_H
#define _CHIPSTACK_H

#include <stdint.h>

#define CHIP_STACK_SIZE 16


typedef struct {
    uint16_t *sp;  // stack pointer
	//uint8_t index;
	uint16_t data[CHIP_STACK_SIZE];
} ChipStack;

#endif

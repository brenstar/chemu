#ifndef _CHIP_STACK_H
#define _CHIP_STACK_H

#include <stdint.h>
#include <stdbool.h>

#define CHIP_STACK_SIZE 16

//typedef uint16_t[16] ChipStack;


typedef struct {
    uint16_t *sp;  // stack pointer
	//uint8_t index;
	uint16_t data[CHIP_STACK_SIZE];
} ChipStack;

void chipstack_init(ChipStack *stack);

bool chipstack_can_push(ChipStack *stack);

bool chipstack_can_pop(ChipStack *stack);

void chipstack_push(ChipStack *stack, uint16_t element);

uint16_t chipstack_pop(ChipStack *stack);




#endif

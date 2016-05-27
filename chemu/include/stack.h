#ifndef _STACK_H
#define _STACK_H

#include <stdint.h>
#include <stdbool.h>

#include "ChipStack.h"

void chipstack_init(ChipStack *stack);

bool chipstack_can_push(ChipStack *stack);

bool chipstack_can_pop(ChipStack *stack);

void chipstack_push(ChipStack *stack, uint16_t element);

uint16_t chipstack_pop(ChipStack *stack);




#endif

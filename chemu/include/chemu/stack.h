#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

#include "ChipStack.h"

void chipstack_init(ChipStack *stack);

bool chipstack_can_push(ChipStack *stack);

bool chipstack_can_pop(ChipStack *stack);

void chipstack_push(ChipStack *stack, ChipAddress element);

ChipAddress chipstack_pop(ChipStack *stack);

#if defined(_INLINE_) && defined(_MSC_VER)
#include "inline.h"
#include "stack.ipp"
#endif



#endif

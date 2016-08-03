#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

#include "ChipStack.h"
#include "export.h"

CHEMU_API void chipstack_init(ChipStack *stack);

CHEMU_API bool chipstack_can_push(ChipStack *stack);

CHEMU_API bool chipstack_can_pop(ChipStack *stack);

CHEMU_API void chipstack_push(ChipStack *stack, ChipAddress element);

CHEMU_API ChipAddress chipstack_pop(ChipStack *stack);

#if defined(_INLINE_) && defined(_MSC_VER)
#include "inline.h"
#include "stack.ipp"
#endif



#endif

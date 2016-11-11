#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

#include "ChipStack.h"
#include "internal/export.h"

CHEMU_API void chemu_stack_init(ChipStack *stack);

CHEMU_API bool chemu_stack_canPush(ChipStack *stack);

CHEMU_API bool chemu_stack_canPop(ChipStack *stack);

CHEMU_API void chemu_stack_push(ChipStack *stack, ChipAddress element);

CHEMU_API ChipAddress chemu_stack_pop(ChipStack *stack);

#if defined(_INLINE_) && defined(_MSC_VER)
#include "internal/inline.h"
#include "stack.ipp"
#endif



#endif

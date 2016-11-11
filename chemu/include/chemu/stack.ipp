#include "stack.h"

INLINE void chemu_stack_init(ChipStack *stack) {
    stack->index = CHIP_STACK_SIZE - 1;
}

INLINE bool chemu_stack_canPop(ChipStack *stack) {
    return stack->index != CHIP_STACK_SIZE - 1;
}

INLINE bool chemu_stack_canPush(ChipStack *stack) {
    return stack->index != 0;
}

INLINE void chemu_stack_push(ChipStack *stack, ChipAddress element) {
    stack->data[stack->index--] = element;
}

INLINE ChipAddress chemu_stack_pop(ChipStack *stack) {
    return stack->data[++(stack->index)];
}

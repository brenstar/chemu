
#include "chemu/stack.h"


inline void chipstack_init(ChipStack *stack) {
    stack->index = CHIP_STACK_SIZE;
}

inline bool chipstack_can_pop(ChipStack *stack) {
    return stack->index != CHIP_STACK_SIZE;
}

inline bool chipstack_can_push(ChipStack *stack) {
    return stack->index != 0;
}

void chipstack_push(ChipStack *stack, ChipAddress element) {
    stack->data[stack->index--] = element;
}

ChipAddress chipstack_pop(ChipStack *stack) {
    return stack->data[++(stack->index)];
}

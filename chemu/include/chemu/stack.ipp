#include "stack.h"

INLINE void chipstack_init(ChipStack *stack) {
	stack->index = CHIP_STACK_SIZE - 1;
}

INLINE bool chipstack_can_pop(ChipStack *stack) {
	return stack->index != CHIP_STACK_SIZE - 1;
}

INLINE bool chipstack_can_push(ChipStack *stack) {
	return stack->index != 0;
}

INLINE void chipstack_push(ChipStack *stack, ChipAddress element) {
	stack->data[stack->index--] = element;
}

INLINE ChipAddress chipstack_pop(ChipStack *stack) {
	return stack->data[++(stack->index)];
}

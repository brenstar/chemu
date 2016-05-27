
#include "stack.h"


inline void chipstack_init(ChipStack *stack) {
    stack->sp = stack->data + CHIP_STACK_SIZE;
}

inline bool chipstack_can_pop(ChipStack *stack) {
    return stack->sp != stack->data + CHIP_STACK_SIZE;
}

inline bool chipstack_can_push(ChipStack *stack) {
    return stack->sp != stack->data;
}


void chipstack_push(ChipStack *stack, uint16_t element) {
    //stack->data[stack->sp--] = element;
    *(stack->sp--) = element;
}

uint16_t chipstack_pop(ChipStack *stack) {
    return *(++stack->sp);
    //return stack->data[--(stack->sp)];
}

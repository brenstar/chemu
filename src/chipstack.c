
#include "chipstack.h"


inline bool chipstack_can_pop(ChipStack stack) {
    return stack->index > 0;
}

inline bool chipstack_can_push(ChipStack stack) {
    return stack->index < CHIP_STACK_SIZE;
}


void chipstack_push(ChipStack stack, uint16_t element) {

    if (stack->index < CHIP_STACK_SIZE)
        stack->data[stack->index++] = element;

}

uint16_t chipstack_pop(ChipStack stack) {
    uint16_t val = 0;
    if (stack->index > 0)
        val = stack->data[--(stack->index)];
    return val;
}

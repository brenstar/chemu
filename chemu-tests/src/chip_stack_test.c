
#include <stdio.h>
#include <stdlib.h>

#include "chemu/stack.h"
#include "chemu/ChipAddress.h"

int main() {
    printf("chipstack size: %d\n", sizeof(ChipStack));

    ChipStack *stack = (ChipStack*)malloc(sizeof(ChipStack));
    chipstack_init(stack);

    printf("Stack array is located at %p\n", stack->data);
    printf("index = %d\n", stack->index);

    printf("\nTesting push...\n");

    ChipAddress sampleData = 0x0F00;
    while(chipstack_can_push(stack)) {

        printf("Pushing 0x%03x ", sampleData);
        chipstack_push(stack, sampleData++);
        printf("at index %d\n", stack->index);
    }

    printf("\nTesting pop...\n");

    while (chipstack_can_pop(stack)) {
        printf("Popped 0x%03x from index %d\n", chipstack_pop(stack), stack->index);
    }

    printf("\nindex = %d\n", stack->index);

    free(stack);

    return 0;
}

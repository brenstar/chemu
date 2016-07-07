
#include <stdio.h>

#include "chemu/stack.h"
#include "chemu/ChipAddress.h"

int main() {
    printf("chipstack size: %d\n", sizeof(ChipStack));

    ChipStack stack;
    chipstack_init(&stack);

    printf("Stack array is located at %p\n", stack.data);
    printf("sp = %p\n", stack.sp);

    printf("\nTesting push...\n");

    ChipAddress sampleData = 0x0F00;
    while(chipstack_can_push(&stack)) {

        printf("Pushing 0x%03x at location %p\n", sampleData, stack.sp);
        chipstack_push(&stack, sampleData++);
    }

    printf("\nTesting pop...\n");

    while (chipstack_can_pop(&stack)) {
        printf("Popped 0x%03x from location %p\n", chipstack_pop(&stack), stack.sp);
    }

    printf("\nsp = %p\n", stack.sp);

    return 0;
}

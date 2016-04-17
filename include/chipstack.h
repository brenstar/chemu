#ifndef _CHIPSTACK_H
#define _CHIPSTACK_H

#include <stdint.h>
#include <stdbool.h>

#include "chiptypes.h"


bool chipstack_can_push(ChipStack stack);

bool chipstack_can_pop(ChipStack stack);

void chipstack_push(ChipStack stack, uint16_t element);

uint16_t chipstack_pop(ChipStack stack);




#endif

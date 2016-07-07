#ifndef _INPUT_H
#define _INPUT_H

#include "ChipInput.h"


//typedef ChipKey (*pollKeyCallback)();

//void chipin_poll(ChipInput *input);

ChipKeyState chipin_keystate(ChipInput *input, ChipKey key);

void chipin_set(ChipInput *input, ChipKey key, ChipKeyState state);

#endif

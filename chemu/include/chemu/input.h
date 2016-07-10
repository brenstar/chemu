#ifndef _INPUT_H
#define _INPUT_H

#include "ChipInput.h"

/**
 * Gets the keystate of a given key stored in the input parameter.
 */
ChipKeyState chipin_keystate(ChipInput *input, ChipKey key);

/**
 * Sets the keystate (pressed or released) of the given key in the input
 * parameter
 */
void chipin_set(ChipInput *input, ChipKey key, ChipKeyState state);

#endif

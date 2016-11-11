#ifndef _INPUT_H
#define _INPUT_H

#include "ChipInput.h"
#include "internal/export.h"

//
// Gets the keystate of a given key stored in the input parameter.
//
CHEMU_API ChipKeyState chemu_in_keystate(ChipInput *input, ChipKey key);

//
// Sets the keystate (pressed or released) of the given key in the input
// parameter
//
CHEMU_API void chemu_in_set(ChipInput *input, ChipKey key, ChipKeyState state);

#endif

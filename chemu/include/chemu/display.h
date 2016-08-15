#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "ChipDisplay.h"
#include "ChipSprite.h"
#include "export.h"

#include <stdbool.h>

// Sets all pixels in the display to 0
CHEMU_API void chipdisplay_clear(ChipDisplay *display);

// Draws the given sprite onto the given display using XOR drawing. If any
// pixels were erased by this operation (1 -> 0) true is returned, false
// otherwise.
CHEMU_API bool chipdisplay_draw(ChipDisplay *display, ChipSprite sprite);

// Gets the state of a pixel at the given coordinates in the display.
CHEMU_API int chipdisplay_get(ChipDisplay *display, int x, int y);

#endif

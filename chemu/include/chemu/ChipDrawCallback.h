// DEPRECATED
// framebuffer is now managed internally (see display.h)

#ifndef _CHIPDRAWCALLBACK_H
#define _CHIPDRAWCALLBACK_H

#include <stdint.h>
#include "ChipSprite.h"

typedef enum {
    CHIP_DRAW_CLEAR,
    CHIP_DRAW_SPRITE
} ChipDrawOp;

typedef enum {
    CHIP_DRAW_NO_COLLISION,
    CHIP_DRAW_COLLISION
} ChipDrawResult;

typedef ChipDrawResult (*ChipDrawCallback)(ChipDrawOp, ChipSprite*);

#endif

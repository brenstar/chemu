#ifndef _CHIPDRAWCALLBACK_H
#define _CHIPDRAWCALLBACK_H

#include <stdint.h>

/**
 * Structure definition for a CHIP-8 sprite. This struct is used when graphics
 * handling is handled externally via callback function.
 */
typedef struct ChipSprite_s {
    uint16_t x:6;        // x coordinate to draw sprite (0-63)
    uint16_t y:5;        // y coordinate to draw sprite (0-31)
    uint16_t rows:4;     // number of rows to draw (1-15)
    uint8_t data[15];   // sprite data
} ChipSprite;

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

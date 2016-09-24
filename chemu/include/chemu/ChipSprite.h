#ifndef _CHIPSPRITE_H
#define _CHIPSPRITE_H

#include <stdint.h>

/**
 * Structure definition for a CHIP-8 sprite. This struct is used when graphics
 * handling is handled externally via callback function.
 */
typedef struct ChipSprite_s {
    uint8_t x;          // x coordinate to draw sprite (0-63)
    uint8_t y;          // y coordinate to draw sprite (0-31)
    uint8_t rows;       // number of rows to draw (1-15)
    uint8_t data[15];   // sprite data
} ChipSprite;

#endif

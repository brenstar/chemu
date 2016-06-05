#ifndef _CHIPDISPLAY_H
#define _CHIPDISPLAY_H

#include <stdint.h>

#define CHIP_DRAW_COLLISION 1

typedef struct ChipSprite_s {
    uint8_t x:6;        // x coordinate to draw sprite (0-63)
    uint8_t y:5;        // y coordinate to draw sprite (0-31)
    uint8_t rows:4;     // number of rows to draw (1-15)
    uint8_t data[15];   // sprite data
} ChipSprite;

typedef enum ChipDrawOp_e {
    CHIP_DRAW_CLEAR,
    CHIP_DRAW_SPRITE
} ChipDrawOp;

typedef int (*DrawHandler)(ChipDrawOp, ChipSprite*);


//typedef uint32_t ChipDisplay[64];

#endif

#include "chemu/display.h"


void chipdisplay_clear(ChipDisplay display) {
    for (int i = 0; i < CHIP_DISPLAY_COLS; ++i)
        display[i] = 0;
}


bool chipdisplay_draw(ChipDisplay display, ChipSprite sprite) {
    bool collision = false;
    for (int row = 0; row < sprite.rows; ++row) {
        // get the sprite data from the row index
        uint8_t spriteData = sprite.data[row];
        // shift the sprite data by the x coordinate
        int64_t spriteRow = spriteData << sprite.x;
        // rotate if necessary (so sprites will wrap)
        if (sprite.x > CHIP_DISPLAY_COLS - 8)
            spriteRow |= spriteData >> (CHIP_DISPLAY_COLS - sprite.x);

        // determine y coordinate, wrap if necessary
        int y = (sprite.y + row) & (CHIP_DISPLAY_ROWS - 1);

        // get the current row in the display (for collision test)
        int64_t oldRow = display[y];

        // draw the row
        display[y] ^= spriteRow;

        // collision checking
        collision = !collision && ((oldRow & display[y]) != oldRow);
    }
    return collision;
}

int chipdisplay_get(ChipDisplay display, int x, int y) {
    return (display[y] >> x) & 1;
}

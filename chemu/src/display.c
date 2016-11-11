#include "chemu/display.h"


void chemu_disp_clear(ChipDisplay *display) {
    for (int i = 0; i < CHIP_DISPLAY_ROWS; ++i) {
        // inner for loop not used for optimization
        display->buffer[i][0] = 0;
        display->buffer[i][1] = 0;
    }
}

bool chemu_disp_draw(ChipDisplay *display, ChipSprite sprite) {
    bool collision = false;

    const int x = sprite.x & 31;
    //int xf_right = 32 - x;
    //int xf_left = xf_right + 8;
    const int i = sprite.x >= 32; // left or right?
    const bool wrap = x > 24;

    const int shift = wrap ? (x - 24) : (24 - x);

    //int b = 31 - x;

    for (int row = 0; row < sprite.rows; ++row) {

        // get the sprite data to draw
        uint8_t spriteData = sprite.data[row];

        // row of data to draw, 0=left 1=right
        int32_t spriteRow[] = {0, 0};

        // shift sprite data by x
        //spriteRow[i] = spriteData << xf;
        // rotate if necessary onto other side
        if (wrap) {
            spriteRow[i] = spriteData >> shift;
            spriteRow[!i] = spriteData << (32 - shift);
        } else {
           spriteRow[i] = spriteData << shift;
        }
            //spriteRow[!i] = spriteData >> (32);

        // a & b is equal to a % b when b = 2^k - 1 for any k
        // calculate y, wrap if necessary
        int y = (sprite.y + row) & (CHIP_DISPLAY_ROWS - 1);

        // iterate both sides of the row
        for (int i = 0; i != 2; ++i) {
            int32_t oldRow = display->buffer[y][i];  // get the old row
            int32_t newRow = oldRow ^ spriteRow[i];  // calculate new row
            display->buffer[y][i] = newRow;         // update the row
            // collision checking, a collision occurred if any pixels were
            // erased, 1 -> 0. A Bitwise AND of the old row and the new row
            // indicates a collison if the result is not equal to the old row
            if (!collision)
                collision = (oldRow & newRow) != oldRow;
        }
    }

    return collision;

}

int chemu_disp_get(ChipDisplay *display, int x, int y) {
    // bounds checking omitted
    return (display->buffer[y][x >> 5] >> (31 - (x & 31))) & 1;
}

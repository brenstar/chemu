#ifndef _CHIPDISPLAY_H
#define _CHIPDISPLAY_H

#define CHIP_DISPLAY_ROWS 32
#define CHIP_DISPLAY_COLS 64

#include <stdint.h>

/**
 * Typedef for a CHIP-8 framebuffer as an array of 64 32-bit unsigned intgers.
 * Each integer represents a column in the buffer, with each bit representing
 * the state of a pixel.
 */
typedef int64_t ChipDisplay[CHIP_DISPLAY_ROWS];
//typedef uint32_t ChipDisplay[CHIP_DISPLAY_COLS];

#endif

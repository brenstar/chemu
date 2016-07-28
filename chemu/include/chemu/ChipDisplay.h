#ifndef _CHIPDISPLAY_H
#define _CHIPDISPLAY_H

#define CHIP_DISPLAY_ROWS 32
#define CHIP_DISPLAY_COLS 64

// required size (in bytes) of the ChipDisplay type
#define CHIP_DISPLAY_SIZE 256

#include <stdint.h>


/*
 * Typedef for a CHIP-8 framebuffer. The ChipDisplay type is a struct with one
 * member, buffer. The buffer member is a 2D array of 32-bit integers with
 * dimensions 32 (number of rows in a CHIP-8 display) and 2.
 */
typedef struct ChipDisplay_s {
    int32_t buffer[CHIP_DISPLAY_ROWS][2];
} ChipDisplay;


//typedef int64_t ChipDisplay[CHIP_DISPLAY_ROWS];
//typedef uint32_t ChipDisplay[CHIP_DISPLAY_COLS];

#endif

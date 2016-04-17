#ifndef _DISPLAY_H
#define _DISPLAY_H

#define CHIP_DISPLAY_ROWS 32
#define CHIP_DISPLAY_COLS 64

// Display ADT
#ifndef CHIP_DISPLAY_ADT
typedef struct {} *ChipDisplay;
#endif

ChipDisplay display_create();

void display_clear(ChipDisplay display);

void display_set(ChipDisplay display, int x, int y, bool state);

bool display_get(ChipDisplay display, int x, int y);

void display_destroy(ChipDisplay display);



#endif

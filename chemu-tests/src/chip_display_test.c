
#include "chemu/display.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t flag = 0;

void interruptHandler(int sig) {
    flag = 1;
}


#define setCursor(row, column) printf("\033[%d;%dH", row, column);

static const char ON_CHAR = '1', OFF_CHAR = ' ';

static const char* VBORDER = "+----------------------------------------------------------------+";
//static const char HBORDER = '|';
static const char* HBORDER = "|                                                                |";

static void dumpDisplay(ChipDisplay *display);


//int main(int argc, const char* argv[]) {
int main() {
    signal(SIGINT, interruptHandler);

    printf("\033[2J");
    fflush(stdout);
    setCursor(1, 1);

    printf("ChipDisplay size: %d (expected: %d)\n", sizeof(ChipDisplay), CHIP_DISPLAY_SIZE);
    assert(sizeof(ChipDisplay) == CHIP_DISPLAY_SIZE);
    ChipDisplay *display = (ChipDisplay*)malloc(sizeof(ChipDisplay));


    puts("Clearing display. Checking buffer contents...");
    chipdisplay_clear(display);
    for (int i = 0; i < CHIP_DISPLAY_ROWS; ++i) {
        assert(display->buffer[i][0] == 0);
        assert(display->buffer[i][1] == 0);
    }

    puts("Testing Display. Ctrl-C to exit");
    puts(VBORDER);
    for (int i = 0; i < CHIP_DISPLAY_ROWS; ++i)
        puts(HBORDER);
    puts(VBORDER);

    fflush(stdout);

    ChipSprite sprite = {
        .x = 0,
        .y = 0,
        .rows = 7,
        .data = {
            24, 60, 126, 255, 126, 60, 24, 0, 0, 0, 0, 0, 0, 0, 0
        }
    };

    ChipSprite blockSprite = { .x = 40, .y = 0, .rows = 8};
    for (int i = 0; i < 8; ++i)
        blockSprite.data[i] = 0xFF;

    chipdisplay_draw(display, sprite);

    // for (int r = 0; r < 4; ++r) {
    //     for (int c = 0; c < 4; ++c) {
    //         chipdisplay_draw(display, blockSprite);
    //         blockSprite.x += 16;
    //     }
    //     blockSprite.x = (r % 2) == 0 ? 0 : 8;
    //     blockSprite.y += 8;
    // }

    for (int i = 0; i < 4; ++i) {
        chipdisplay_draw(display, blockSprite);
        blockSprite.y += 8;
    }


    while (!flag) {
        dumpDisplay(display);
        chipdisplay_draw(display, sprite);
        sprite.x += 2;
        sprite.y += 1;
        bool collision = chipdisplay_draw(display, sprite);
        setCursor(38, 1);
        printf("Collision: %d", collision);
        fflush(stdout);
        usleep(250000);
    }


    //
    // if (argc == 3) {
    //     sprite.x = (uint8_t)strtol(argv[1], NULL, 10);
    //     sprite.y = (uint8_t)strtol(argv[2], NULL, 10);
    // }
    //
    // //for (int i = 0; i < 2; ++i) {
    //     printf("Drawing sprite, collision: %d\n", chipdisplay_draw(display, sprite));
    //     dumpDisplay(display);
    // //}

    setCursor(39, 1);
    free(display);
    return 0;
}


static void dumpDisplay(ChipDisplay *display) {
    for (int r = 0; r < CHIP_DISPLAY_ROWS; ++r) {
        setCursor(5 + r, 2);
        for (int c = 0; c < CHIP_DISPLAY_COLS; ++c) {
            putchar(chipdisplay_get(display, c, r) ? ON_CHAR : OFF_CHAR);
            fflush(stdout);
        }

    }
}

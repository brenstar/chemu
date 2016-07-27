
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "chemu.h"

//static void printDisplay(ChipDisplay *display, WINDOW *win);
static void redrawHandler(ChipEmu* emu);
static ChipKey getKeyHandler(ChipEmu* emu);

static WINDOW *displayWindow;

int main(int argc, const char * argv[]) {

    if (argc != 3) {
        fputs("usage: chemu-cli <romfile> <speed>", stderr);
        return EXIT_FAILURE;
    }

    ChipEmu *emu = (ChipEmu*)malloc(sizeof(ChipEmu));
    chipemu_init(emu);

    emu->redrawCallback = redrawHandler;
    emu->pollKeyHandler = getKeyHandler;

    emu->speed = (int)strtol(argv[2], NULL, 10);


    int romsize = chipemu_loadROM(emu, argv[1]);
    if (romsize == -1) {
        fprintf(stderr, "Failed to load ROM\n");
        return EXIT_FAILURE;
    }
    printf("ROM size: %d bytes\n", romsize);

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    displayWindow = newwin(CHIP_DISPLAY_ROWS + 2, CHIP_DISPLAY_COLS * 2 + 2, 0, 0);
    wborder(displayWindow, '|', '|', '-', '-', '+', '+', '+', '+');

    refresh();
    wrefresh(displayWindow);

    //getch();
    chipemu_mainLoop(emu);
    getch();

    delwin(displayWindow);
    endwin();

    free(emu);

    return 0;
}

static void redrawHandler(ChipEmu *emu) {
    for (int y = 0; y < CHIP_DISPLAY_ROWS; ++y) {
        wmove(displayWindow, y + 1, 1);
        for (int x = 0; x < CHIP_DISPLAY_COLS; ++x) {
            int pixel = chipdisplay_get(&emu->memory.reserved.display, x, y);
            wattrset(displayWindow, pixel ? A_STANDOUT : A_NORMAL);
            waddstr(displayWindow, "  ");
        }
    }
    wrefresh(displayWindow);
}

static ChipKey getKeyHandler(ChipEmu *emu) {
    return CHIP_KEY_0;
}

// static void printDisplay(ChipDisplay *display, WINDOW *win) {
//
//     //mvprintw(row, col, VBORDER);
//     //wmove(win, 0, 0);
//     for (int y = 0; y < CHIP_DISPLAY_ROWS; ++y) {
//         wmove(win, y + 1, 1);
//         for (int x = 0; x < CHIP_DISPLAY_COLS; ++x) {
//             int pixel = chipdisplay_get(display, x, y);
//             wattrset(win, pixel ? A_STANDOUT : A_NORMAL);
//             waddstr(win, " ");
//             //addstr(pixel ? ON_STR : OFF_STR);
//         }
//         //addch('|');
//     }
//     //mvprintw(row + CHIP_DISPLAY_ROWS + 1, col, VBORDER);
// }

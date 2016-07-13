
#include <ncurses.h>
#include <stdlib.h>

#include "chemu/display.h"

// static const char* VBORDER = "+----------------------------------------------------------------+\n";
//
// static const char ON_CHAR = '1', OFF_CHAR = ' ';
//
// static const char* ON_STR = "@@";
// static const char* OFF_STR = "  ";

static void printDisplay(ChipDisplay *display, WINDOW *win);

int main() {

    ChipDisplay *display = (ChipDisplay*)malloc(sizeof(ChipDisplay));
    chipdisplay_clear(display);

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    WINDOW *displayWindow = newwin(CHIP_DISPLAY_ROWS + 2, CHIP_DISPLAY_COLS * 2 + 2, 2, 0);
    wborder(displayWindow, '|', '|', '-', '-', '+', '+', '+', '+');

    printw("Use arrow keys to move sprite. Press q to exit.");

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

    for (int i = 0; i < 4; ++i) {
        chipdisplay_draw(display, blockSprite);
        blockSprite.y += 8;
    }

    chipdisplay_draw(display, sprite);

    int ch = 0;
    do {
        chipdisplay_draw(display, sprite); // erase sprite
        switch (ch) {
            case KEY_LEFT:
                sprite.x -= 1;
                break;
            case KEY_RIGHT:
                sprite.x += 1;
                break;
            case KEY_UP:
                sprite.y -= 1;
                break;
            case KEY_DOWN:
                sprite.y += 1;
                break;
            default:
                break;
        }
        int collision = chipdisplay_draw(display, sprite); // draw at new position

        move(1, 0);
        clrtoeol();
        printw("X: %d     Y: %d     Collision: %d", sprite.x, sprite.y, collision);
        refresh();

        printDisplay(display, displayWindow);
        wrefresh(displayWindow);

    } while ((ch = getch()) != 'q');

    delwin(displayWindow);
    endwin();

    free(display);

    return 0;
}

static void printDisplay(ChipDisplay *display, WINDOW *win) {

    //mvprintw(row, col, VBORDER);
    //wmove(win, 0, 0);
    for (int y = 0; y < CHIP_DISPLAY_ROWS; ++y) {
        wmove(win, y + 1, 1);
        for (int x = 0; x < CHIP_DISPLAY_COLS; ++x) {
            int pixel = chipdisplay_get(display, x, y);
            wattrset(win, pixel ? A_STANDOUT : A_NORMAL);
            waddstr(win, "  ");
            //addstr(pixel ? ON_STR : OFF_STR);
        }
        //addch('|');
    }
    //mvprintw(row + CHIP_DISPLAY_ROWS + 1, col, VBORDER);
}

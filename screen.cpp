//
// Created by michal on 16.10.17.
//

#include <ncurses.h>
#include "screen.h"

int init_screen() {

    // initialize ncurses
    initscr();
    // disable console buffering
    if (cbreak() != OK)
        return -1;
    // do not show typed characters
    if (noecho() != OK)
        return -1;
    // enable sensing all keys as single values
    if (keypad(stdscr, true) != OK)
        return -1;

    //nodelay(stdscr, true);

    // make cursor invisible
    if (curs_set(0) == ERR)
        return -1;

    mvprintw(0, 2, "Throttle: ");
    mvprintw(1, 2, "Roll: ");
    mvprintw(1, 22, "Pitch: ");
    mvprintw(1, 42, "Yaw: ");

    print_throttle(0);
    print_euler(0, 0, 0);

    return 0;
}

void print_euler(float roll, float pitch, float yaw) {
    mvprintw(1, 12, "%8.3f", roll);
    mvprintw(1, 32, "%8.3f", pitch);
    mvprintw(1, 52, "%8.3f", yaw);
}

void print_throttle(uint16_t throttle) {
    mvprintw(0, 16, "%4d", throttle);
}

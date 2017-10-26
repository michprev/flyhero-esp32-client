//
// Created by michal on 16.10.17.
//

#include <ncurses.h>
#include "screen.h"
#include "shared_data.h"


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

    // make getch non-blocking
    nodelay(stdscr, true);

    // make cursor invisible
    if (curs_set(0) == ERR)
        return -1;

    mvprintw(0, 2, "Throttle: ");
    mvprintw(0, 65, "CPU: ");
    mvprintw(1, 2, "Roll: ");
    mvprintw(1, 22, "Pitch: ");
    mvprintw(1, 42, "Yaw: ");

    print_throttle();
    print_euler();
    print_cpu_load();

    return 0;
}

void print_euler() {
    Euler_Data euler = Get_Euler();

    mvprintw(1, 12, "%8.3f", euler.roll);
    mvprintw(1, 32, "%8.3f", euler.pitch);
    mvprintw(1, 52, "%8.3f", euler.yaw);
}

void print_throttle() {
    uint16_t throttle = Get_Throttle();

    mvprintw(0, 16, "%4d", throttle);
}

void print_cpu_load() {
    double load = Get_CPU_Load();

    mvprintw(0, 70, "%4.1f", load);
}

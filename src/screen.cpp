//
// Created by michal on 16.10.17.
//

#include <ncurses.h>
#include <cstdlib>
#include "screen.h"
#include "shared_data.h"


int init_screen()
{
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

    return 0;
}

MENU *print_tcp_menu()
{
    ITEM **my_menu_items = (ITEM **)calloc(4, sizeof(ITEM *));

    my_menu_items[0] = new_item("Calibrate", "");
    set_item_userptr(my_menu_items[0], (void *) "Calibrate");
    my_menu_items[1] = new_item("Start", "");
    set_item_userptr(my_menu_items[1], (void *) "Start");
    my_menu_items[2] = new_item("Exit", "");
    set_item_userptr(my_menu_items[2], (void *) "Exit");
    my_menu_items[3] = NULL;

    MENU *commands_menu = new_menu(my_menu_items);
    set_menu_mark(commands_menu, "* ");

    post_menu(commands_menu);
    refresh();

    return commands_menu;
}

void free_tcp_menu(MENU *menu)
{
    int i = 0;

    while(menu->items[i] != NULL)
        free_item(menu->items[i++]);

    free_menu(menu);
}

void print_euler()
{
    Euler_Data euler = Get_Euler();

    mvprintw(1, 12, "%8.3f", euler.roll);
    mvprintw(1, 32, "%8.3f", euler.pitch);
    mvprintw(1, 52, "%8.3f", euler.yaw);
}

void print_throttle()
{
    uint16_t throttle = Get_Throttle();

    mvprintw(0, 16, "%4d", throttle);
}

void print_cpu_load()
{
    double load = Get_CPU_Load();

    mvprintw(0, 70, "%4.1f", load);
}

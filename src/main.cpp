#include <iostream>
#include <thread>
#include <ncurses.h>
#include <menu.h>
#include <future>
#include <chrono>
#include <cstring>
#include <unistd.h>

#include "shared_data.h"
#include "logger.h"
#include "screen.h"
#include "udp_client.h"
#include "tcp_client.h"


using namespace std;

int main(int argc, char **argv)
{
    int key;

    if (init_screen() != 0)
        return 1;

    MENU *menu = print_tcp_menu();

    if (!tcp_connect())
    {
        free_tcp_menu(menu);
        endwin();

        cerr << "Could not open tcp socket" << endl;

        return 1;
    }

    bool tcp_loop = true;

    while (tcp_loop)
    {
        switch (key = wgetch(stdscr))
        {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10:
            {
                char *text = (char *)item_userptr(current_item(menu));

                if (strcmp(text, "Calibrate") == 0)
                {
                    if (tcp_send("calibrate", strlen("calibrate")))
                        mvprintw(20, 2, "Calibration OK");
                    else
                        mvprintw(20, 2, "Calibration failed");
                } else if (strcmp(text, "Start") == 0)
                {
                    if (tcp_send("start", strlen("start")))
                    {
                        mvprintw(20, 2, "Start OK");
                        tcp_loop = false;
                    } else
                        mvprintw(20, 2, "Start failed");
                } else if (strcmp(text, "Exit") == 0)
                {
                    free_tcp_menu(menu);
                    endwin();
                    return 0;
                }

                break;
            }
            default:
                if (key != -1)
                    mvprintw(20, 2, "unrecognized key %d", key);
                break;
        }
    }

    free_tcp_menu(menu);

    clear();

    mvprintw(0, 2, "Throttle: ");
    mvprintw(0, 65, "CPU: ");
    mvprintw(1, 2, "Roll: ");
    mvprintw(1, 22, "Pitch: ");
    mvprintw(1, 42, "Yaw: ");

    print_throttle();
    print_euler();
    print_cpu_load();

    usleep(100000);

    if (!udp_connect())
        return 1;

    if (open_log() != 0)
        cerr << "Unable to open log file" << endl;

    thread udp_thread(udp_task);

    uint16_t throttle;

    while (Get_Run())
    {
        print_throttle();
        print_euler();
        print_cpu_load();

        switch (key = wgetch(stdscr))
        {
            case -1:

                break;
            case 5:
                Set_Run(false);

                break;
            case KEY_UP:
                throttle = Get_Throttle();

                if (throttle != 1000)
                    Set_Throttle(throttle + 10);

                break;
            case KEY_DOWN:
                throttle = Get_Throttle();

                if (throttle != 0)
                    Set_Throttle(throttle - 10);

                break;
            default:
                mvprintw(20, 2, "unrecognized key %d", key);
                break;
        }
    }

    udp_thread.join();

    close_log();

    endwin();

    return 0;
}

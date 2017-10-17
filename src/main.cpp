#include <iostream>
#include <thread>
#include <ncurses.h>
#include <future>
#include <chrono>

#include "shared_data.h"
#include "screen.h"
#include "udp_client.h"

using namespace std;


int main(int argc, char **argv) {

    if (init_screen() != 0)
        return 1;

    if (udp_connect() != 0)
        return 1;

    thread udp_thread(udp_task);

    int key;
    uint16_t throttle;

    while (Get_Run()) {
        switch (key = wgetch(stdscr)) {
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
                cout << "default key: " << key << endl;
                break;
        }
    }

    udp_thread.join();

    endwin();

    return 0;
}

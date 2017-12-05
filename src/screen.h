//
// Created by michal on 16.10.17.
//

#pragma once


#include <menu.h>


int init_screen();

MENU* print_tcp_menu();
void free_tcp_menu(MENU *menu);

void print_euler();
void print_throttle();
void print_cpu_load();

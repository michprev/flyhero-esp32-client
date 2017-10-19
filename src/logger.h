//
// Created by michal on 17.10.17.
//

#pragma once

#include "shared_data.h"


int open_log();

void close_log();

void log_data(Euler_Data *euler, int euler_count);
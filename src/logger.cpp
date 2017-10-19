//
// Created by michal on 17.10.17.
//

#include <fstream>
#include <ctime>
#include "logger.h"
#include "shared_data.h"

using namespace std;

ofstream log_file;
char file_name[50];
bool header_written = false;
clock_t start_time;

int open_log() {
    time_t now;
    time(&now);

    tm *local_now;
    local_now = localtime(&now);

    strftime(file_name, 50, "%d_%m_%Y_%H_%M.csv", local_now);

    log_file.open(file_name, ios_base::out);

    return (log_file.is_open() ? 0 : -1);
}

void log_data(Euler_Data *euler, int euler_count) {
    uint16_t throttle = Get_Throttle();

    if (!header_written) {
        header_written = true;
        start_time = clock();

        log_file << "Time;Throttle";

        for (int i = 0; i < euler_count; i++)
            log_file << ";Roll" << i << ";Pitch" << i << ";Yaw" << i;
        log_file << endl;


        log_file << 0 << ';' << throttle;

        for (int i = 0; i < euler_count; i++)
            log_file << ';' << euler[i].roll << ';' << euler[i].pitch << ';' << euler[i].yaw;

        log_file << endl;
    }
    else {
        log_file << 1000.0 * (clock() - start_time) / CLOCKS_PER_SEC << ';' << throttle;

        for (int i = 0; i < euler_count; i++)
            log_file << ';' << euler[i].roll << ';' << euler[i].pitch << ';' << euler[i].yaw;

        log_file << endl;
    }
}

void close_log() {
    log_file.close();

    if (!header_written)
        remove(file_name);
}

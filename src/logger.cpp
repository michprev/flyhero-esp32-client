//
// Created by michal on 17.10.17.
//

#include <fstream>
#include <ctime>
#include "logger.h"

using namespace std;

ofstream log_file;
char file_name[50];
bool header_written = false;

int open_log() {
    time_t now;
    time(&now);

    tm *local_now;
    local_now = localtime(&now);

    strftime(file_name, 50, "%d_%m_%Y_%H_%M.csv", local_now);

    log_file.open(file_name, ios_base::out);

    return (log_file.is_open() ? 0 : -1);
}

void log_euler(float roll, float pitch, float yaw) {
    if (!header_written) {
        log_file << "Roll;Pitch;Yaw" << endl;
        header_written = true;
    }

    log_file << roll << ";" << pitch << ";" << yaw << endl;
}

void close_log() {
    log_file.close();

    if (!header_written)
        remove(file_name);
}

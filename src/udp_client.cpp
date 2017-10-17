//
// Created by michal on 16.10.17.
//

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#include "udp_client.h"
#include "crc.h"
#include "shared_data.h"
#include "screen.h"
#include "logger.h"

int udp_socket = -1;
sockaddr_in server_address;

void parse_message(uint8_t *data, int length);

int udp_connect() {
    using namespace std;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket == -1) {
        cerr << "Could not open udp socket" << endl;
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4789);
    inet_pton(AF_INET, "192.168.4.1", &server_address.sin_addr);

    if (connect(udp_socket, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        cerr << "Could not connect to server" << endl;
        return -1;
    }

    return 0;
}

void udp_task() {
    using namespace std;

    uint8_t data[6];

    const uint8_t READ_BUFFER_LENGTH = 20;
    uint8_t read_buffer[READ_BUFFER_LENGTH];
    int received_count;
    chrono::system_clock::time_point time;

    while (Get_Run()) {
        time = chrono::system_clock::now();
        time += chrono::milliseconds(80);

        uint16_t throttle = Get_Throttle();

        data[0] = 6;
        data[1] = 0x00;
        data[2] = throttle & 0xFF;
        data[3] = throttle >> 8;

        uint16_t crc = crc16(data, 4);

        data[4] = crc & 0xFF;
        data[5] = crc >> 8;

        if (sendto(udp_socket, data, 6, 0, (sockaddr *)&server_address, sizeof(server_address)) == -1)
            cerr << "Could not send message" << endl;

        socklen_t address_length;
        if ( (received_count = recvfrom(udp_socket, read_buffer, READ_BUFFER_LENGTH,
                                        MSG_DONTWAIT, (sockaddr *)&server_address, &address_length)) > 0)
            parse_message(read_buffer, received_count);

        this_thread::sleep_until(time);
    }
}

void parse_message(uint8_t *data, int length) {

    if (length < 4 || data[0] != length) {
        std::cerr << "test" << std::endl;

        return;
    }

    uint16_t crc;
    crc = data[length - 1] << 8;
    crc |= data[length - 2];

    if (crc != crc16(data, length - 2)) {
        std::cerr << "crc error should be " <<  crc16(data, length - 2) << ", is " << crc << std::endl;

        return;
    }


    switch (data[1]) {
        case 0x00:
            Euler_Data euler;

            uint8_t *roll, *pitch, *yaw;
            roll = (uint8_t*)&euler.roll;
            pitch = (uint8_t*)&euler.pitch;
            yaw = (uint8_t*)&euler.yaw;

            roll[0] = data[5];
            roll[1] = data[4];
            roll[2] = data[3];
            roll[3] = data[2];

            pitch[0] = data[9];
            pitch[1] = data[8];
            pitch[2] = data[7];
            pitch[3] = data[6];

            yaw[0] = data[13];
            yaw[1] = data[12];
            yaw[2] = data[11];
            yaw[3] = data[10];


            log_euler(euler.roll, euler.pitch, euler.yaw);

            Set_Euler(euler);

            break;
    }
}

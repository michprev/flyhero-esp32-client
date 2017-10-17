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

int udp_socket = -1;
sockaddr_in server_address;

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

int udp_task() {
    using namespace std;

    uint8_t data[6];

    const uint8_t READ_BUFFER_LENGTH = 20;
    uint8_t read_buffer[READ_BUFFER_LENGTH];
    int received_count;


    while (true) {
        chrono::system_clock::time_point time = chrono::system_clock::now();
        time += chrono::milliseconds(80);

        uint16_t tmp_throttle = Get_Throttle();

        data[0] = 6;
        data[1] = 0x00;

        memcpy(data + 2, &tmp_throttle, sizeof(tmp_throttle));

        uint16_t crc = crc16(data, 4);

        memcpy(data + 4, &crc, sizeof(crc));

        if (sendto(udp_socket, data, 6, 0, (sockaddr *)&server_address, sizeof(server_address)) == -1)
            cerr << "Could not send message" << endl;

        uint32_t tmp;
        if ( (received_count = recvfrom(udp_socket, read_buffer, READ_BUFFER_LENGTH, MSG_DONTWAIT, (sockaddr *)&server_address, &tmp)) > 0) {

            if (read_buffer[0] != received_count)
                continue;

            uint16_t crc = read_buffer[received_count - 2] << 8;
            crc |= read_buffer[received_count - 1];

            if (crc != crc16(read_buffer, received_count - 2))
                continue;


            switch (read_buffer[1]) {
                case 0x00:

                    float roll, pitch, yaw;

                    memcpy(&roll, read_buffer + 2, sizeof(roll));
                    memcpy(&pitch, read_buffer + 6, sizeof(pitch));
                    memcpy(&yaw, read_buffer + 10, sizeof(yaw));

                    print_euler(roll, pitch, yaw);

                    break;
            }
        }

        this_thread::sleep_until(time);
    }
}

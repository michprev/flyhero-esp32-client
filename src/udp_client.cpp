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


static int udp_socket = -1;
static sockaddr_in server_address;

void parse_message(uint8_t *data, int length);

bool udp_connect()
{
    using namespace std;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket == -1)
    {
        cerr << "Could not open udp socket" << endl;
        return false;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4789);
    inet_pton(AF_INET, "192.168.4.1", &server_address.sin_addr);

    if (connect(udp_socket, (sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        cerr << "Could not connect to udp server" << endl;
        return false;
    }

    return true;
}

void udp_task()
{
    using namespace std;

    uint8_t data[11];

    const uint8_t READ_BUFFER_LENGTH = 150;
    uint8_t read_buffer[READ_BUFFER_LENGTH];
    int received_count;
    chrono::system_clock::time_point time;

    while (Get_Run())
    {
        time = chrono::system_clock::now();
        time += chrono::milliseconds(80);

        uint16_t throttle = Get_Throttle();

        data[0] = 11;
        data[1] = throttle & 0xFF;
        data[2] = throttle >> 8;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;
        data[8] = 0;

        uint16_t crc = crc16(data, 9);

        data[9] = crc & 0xFF;
        data[10] = crc >> 8;

        if (sendto(udp_socket, data, 11, 0, (sockaddr *) &server_address, sizeof(server_address)) == -1)
            cerr << "Could not send message" << endl;

        socklen_t address_length;
        if ((received_count = recvfrom(udp_socket, read_buffer, READ_BUFFER_LENGTH,
                                       MSG_DONTWAIT, (sockaddr *) &server_address, &address_length)) > 0)
            parse_message(read_buffer, received_count);

        this_thread::sleep_until(time);
    }
}

void parse_message(uint8_t *data, int length)
{

    if ((length - 5) % 12 != 0 || data[0] != length)
    {
        std::cerr << "test" << std::endl;

        return;
    }

    uint16_t crc;
    crc = data[length - 1] << 8;
    crc |= data[length - 2];

    if (crc != crc16(data, length - 2))
    {
        std::cerr << "crc error should be " << crc16(data, length - 2) << ", is " << crc << std::endl;

        return;
    }


    const int EULER_DATA_COUNT = (length - 5) / 12;

    uint16_t free_time;
    uint8_t *roll, *pitch, *yaw;

    Euler_Data *euler = new Euler_Data[EULER_DATA_COUNT];

    free_time = data[1];
    free_time |= data[2] << 8;

    Set_CPU_Load(100 - free_time / 100.0);

    for (int i = 0; i < EULER_DATA_COUNT; i++)
    {
        roll = (uint8_t *) &(euler[i].roll);
        pitch = (uint8_t *) &(euler[i].pitch);
        yaw = (uint8_t *) &(euler[i].yaw);

        roll[0] = data[3 + i * 12];
        roll[1] = data[4 + i * 12];
        roll[2] = data[5 + i * 12];
        roll[3] = data[6 + i * 12];
        roll[4] = data[7 + i * 12];
        roll[5] = data[8 + i * 12];
        roll[6] = data[9 + i * 12];
        roll[7] = data[10 + i * 12];

        pitch[0] = data[11 + i * 12];
        pitch[1] = data[12 + i * 12];
        pitch[2] = data[13 + i * 12];
        pitch[3] = data[14 + i * 12];
        pitch[4] = data[15 + i * 12];
        pitch[5] = data[16 + i * 12];
        pitch[6] = data[17 + i * 12];
        pitch[7] = data[18 + i * 12];

        yaw[0] = data[19 + i * 12];
        yaw[1] = data[20 + i * 12];
        yaw[2] = data[21 + i * 12];
        yaw[3] = data[22 + i * 12];
        yaw[4] = data[23 + i * 12];
        yaw[5] = data[24 + i * 12];
        yaw[6] = data[25 + i * 12];
        yaw[7] = data[26 + i * 12];
    }

    Set_Euler(euler[0]);

    log_data(euler, EULER_DATA_COUNT);
}

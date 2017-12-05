//
// Created by michal on 27.11.17.
//

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "tcp_client.h"
#include "crc.h"


static int tcp_socket = -1;
static sockaddr_in server_address;

bool tcp_connect()
{
    using namespace std;

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcp_socket == -1)
    {
        return false;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4821);
    inet_pton(AF_INET, "192.168.4.1", &server_address.sin_addr);

    if (connect(tcp_socket, (sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        return false;
    }

    return true;
}

bool tcp_send(const char *command, uint8_t command_length)
{
    uint8_t *data = new uint8_t[command_length + 2];
    strncpy((char*)data, command, command_length);

    uint16_t crc = crc16(data, command_length);

    data[command_length] = crc & 0xFF;
    data[command_length + 1] = crc >> 8;

    if (send(tcp_socket, data, command_length + 2, 0) != command_length + 2)
    {
        delete data;
        return false;
    }

    delete data;

    uint8_t response[5];

    if (recv(tcp_socket, response, 5, 0) != 5)
        return false;

    crc = response[4] << 8;
    crc |= response[3];

    if (crc != crc16(response, 3))
        return false;

    if (strncmp("yup", (char*)response, 3) != 0)
        return false;

    return true;
}
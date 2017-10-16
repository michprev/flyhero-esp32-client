#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <chrono>
#include <ncurses.h>
#include <mutex>

#include <math.h>

using namespace std;

void udp_task();
uint16_t crc16(uint8_t *data, uint8_t length);

void print_euler(float roll, float pitch, float yaw);

uint16_t throttle = 0;
//mutex throttle_mutex; TODO

int main(int argc, char **argv) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);
    
    thread udp_thread(udp_task);

    print_euler(0, 0, 0);
    
    mvprintw(0, 2, "Throttle: ");
    mvprintw(0, 16, "%4d", throttle);
    mvprintw(1, 2, "Roll: ");
    mvprintw(1, 22, "Pitch: ");
    mvprintw(1, 42, "Yaw: ");
    
    int key;

    while (true) {
        switch (key = wgetch(stdscr)) {
            case -1:
                
                break;
            case KEY_UP:
                if (throttle != 1000) {
                    throttle += 10;
                    mvprintw(0, 16, "%4d", throttle);
                }
                break;
            case KEY_DOWN:
                if (throttle != 0) {
                    throttle -= 10;
                    mvprintw(0, 16, "%4d", throttle);
                }
                break;
            default:
                cout << "default key: " << key << endl;
                break;
        }
    }

    return 0;
}

void udp_task() {        
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (udp_socket == -1) {
        cout << "Could not open udp socket" << endl;
        return;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4789);
    inet_pton(AF_INET, "192.168.4.1", &server_addr.sin_addr);
    
    if (connect(udp_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        cout << "Could not connect to server" << endl;
    
    uint8_t data[6];
    
    const uint8_t READ_BUFFER_LENGTH = 20;
    uint8_t read_buffer[READ_BUFFER_LENGTH];
    int received_count;
        
    while (true) {
        chrono::system_clock::time_point time = chrono::system_clock::now();
        time += chrono::milliseconds(80);
        
        data[0] = 6;
        data[1] = 0x00;
        data[2] = throttle >> 8;
        data[3] = throttle & 0xFF;

        uint16_t crc = crc16(data, 4);

        data[4] = crc >> 8;
        data[5] = crc & 0xFF;
        
        if (sendto(udp_socket, data, 6, 0, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
            cout << "Could not send message" << endl;
        
        uint32_t tmp;
        if ( (received_count = recvfrom(udp_socket, read_buffer, READ_BUFFER_LENGTH, MSG_DONTWAIT, (sockaddr *)&server_addr, &tmp)) > 0) {
           
            
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

uint16_t crc16(uint8_t *data, uint8_t length) {
    const uint16_t CRC_POLYNOME = 0x1021;
    uint16_t crc = 0;

    for (uint8_t i = 0; i < length; i++) {
            crc ^= (int16_t)(data[i]) << 8;

            for (uint8_t j = 0; j < 8; j++) {
                    if (crc & 0x8000)
                            crc = (crc << 1) ^ CRC_POLYNOME;
                    else
                            crc = (crc << 1);
            }
    }

    return crc;
}

void print_euler(float roll, float pitch, float yaw) {
    mvprintw(1, 12, "%8.3f", roll);
    mvprintw(1, 32, "%8.3f", pitch);
    mvprintw(1, 52, "%8.3f", yaw);
}
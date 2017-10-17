//
// Created by michal on 17.10.17.
//

#pragma once

struct Euler_Data {
    float roll, pitch, yaw;

    Euler_Data() {
        this->roll = 0;
        this->pitch = 0;
        this->yaw = 0;
    }

    Euler_Data(float roll, float pitch, float yaw) {
        this->roll = roll;
        this->pitch = pitch;
        this->yaw = yaw;
    }
};

uint16_t Get_Throttle();
void Set_Throttle(uint16_t throttle);

bool Get_Run();
void Set_Run(bool run);

Euler_Data Get_Euler();
void Set_Euler(Euler_Data euler);

//
// Created by michal on 17.10.17.
//

#pragma once

struct Euler_Data
{
    double roll, pitch, yaw;

    Euler_Data()
    {
        this->roll = 0;
        this->pitch = 0;
        this->yaw = 0;
    }

    Euler_Data(double roll, double pitch, double yaw)
    {
        this->roll = roll;
        this->pitch = pitch;
        this->yaw = yaw;
    }
};

double Get_CPU_Load();
void Set_CPU_Load(double load);

uint16_t Get_Throttle();
void Set_Throttle(uint16_t throttle);

bool Get_Run();
void Set_Run(bool run);

Euler_Data Get_Euler();
void Set_Euler(Euler_Data euler);

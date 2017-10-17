//
// Created by michal on 17.10.17.
//

#include <mutex>
#include "screen.h"
#include "shared_data.h"

using namespace std;

Euler_Data Euler(0, 0, 0);
timed_mutex Euler_Mutex;

uint16_t Throttle = 0;
timed_mutex Throttle_Mutex;

bool Run = true;

uint16_t Get_Throttle() {
    unique_lock<timed_mutex> throttle_lock(Throttle_Mutex, defer_lock);

    if (!throttle_lock.try_lock_for(chrono::milliseconds(5)))
        return 0;

    uint16_t ret = Throttle;

    throttle_lock.unlock();

    return ret;
}

void Set_Throttle(uint16_t throttle) {
    unique_lock<timed_mutex> throttle_lock(Throttle_Mutex, defer_lock);

    if (!throttle_lock.try_lock_for(chrono::milliseconds(5)))
        return;

    Throttle = throttle;

    throttle_lock.unlock();
}

bool Get_Run() {
    return Run;
}

void Set_Run(bool run) {
    Run = run;
}

Euler_Data Get_Euler() {
    unique_lock<timed_mutex> euler_lock(Euler_Mutex, defer_lock);

    if (!euler_lock.try_lock_for(chrono::milliseconds(5)))
        return Euler_Data();

    Euler_Data ret = Euler;

    euler_lock.unlock();

    return ret;
}

void Set_Euler(Euler_Data euler) {
    unique_lock<timed_mutex> euler_lock(Euler_Mutex, defer_lock);

    if (!euler_lock.try_lock_for(chrono::milliseconds(5)))
        return;

    Euler = euler;

    euler_lock.unlock();
}

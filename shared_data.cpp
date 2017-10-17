//
// Created by michal on 17.10.17.
//

#include <mutex>
#include "screen.h"
#include "shared_data.h"

using namespace std;

uint16_t Throttle;
timed_mutex Throttle_Mutex;

uint16_t Get_Throttle() {
    unique_lock<timed_mutex> throttle_lock(Throttle_Mutex, defer_lock);

    if (!throttle_lock.try_lock_for(chrono::milliseconds(5)))
        return -1;

    uint16_t ret = Throttle;

    throttle_lock.unlock();

    return ret;
}

void Set_Throttle(uint16_t throttle) {
    unique_lock<timed_mutex> throttle_lock(Throttle_Mutex, defer_lock);

    if (!throttle_lock.try_lock_for(chrono::milliseconds(5)))
        return;

    Throttle = throttle;

    print_throttle(Throttle);

    throttle_lock.unlock();
}
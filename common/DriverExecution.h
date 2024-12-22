#ifndef DRIVEREXECUTION_H
#define DRIVEREXECUTION_H

#include <unordered_map>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <chrono>
#include <mutex>
#include <shared_mutex>

static constexpr int16_t delayMicroSeconds = 10000; // 0.01s

namespace common {

class BaseDriver;
class DriverExecution {
public:
    ~DriverExecution();
    static DriverExecution& instance();

    void addDriver(const std::string& clientName, BaseDriver *obj);
    void execute();
    void finialize();
    
private:
    DriverExecution();

    std::unordered_map<std::string, BaseDriver*> mDrivers;
    std::mutex mMutex;
    std::shared_mutex mMutexProcess;

    std::thread mThread;
    useconds_t mDelayMicroSecond = delayMicroSeconds;
    bool mIsThreadRunning{false};
};

}

#endif // DRIVEREXECUTION_H

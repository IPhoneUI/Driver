#ifndef DRIVERFACTORY_H
#define DRIVERFACTORY_H

#include <BaseDriver.h>
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

namespace driver::common {

class DriverFactory {
public:
    ~DriverFactory();
    static DriverFactory& instance();

    template <typename T>
    T *addDriver()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        T* ins = new T();

        std::thread* msqReceiverThread = new std::thread([this, ins] 
        {
            while (true)
            {
                ins->onMsqReceived();
                usleep(mDelayMicroSecond);
            }
        });

        mDrivers.emplace(ins, msqReceiverThread);

        return ins;
    }

    void initialize();
    void finialize();
    void execute();
    
private:
    DriverFactory();

    std::unordered_map<BaseDriver*, std::thread*> mDrivers;
    std::shared_mutex mMutexProcess;

    std::thread mThread;
    std::mutex mMutex;
    useconds_t mDelayMicroSecond = delayMicroSeconds;
    bool mIsThreadRunning{false};
};

}

#endif // DRIVERFACTORY_H

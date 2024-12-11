#ifndef SERVICEFACTORY_H
#define SERVICEFACTORY_H

#include <BaseServiceImpl.h>
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

class ServiceFactory {
public:
    ~ServiceFactory();
    static ServiceFactory& instance();

    template <typename T>
    T *addService()
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

        mServices.emplace(ins, msqReceiverThread);

        return ins;
    }

    void initialize();
    void finialize();
    void execute();
    
private:
    ServiceFactory();

    std::unordered_map<BaseServiceImpl*, std::thread*> mServices;
    std::shared_mutex mMutexProcess;

    std::thread mThread;
    std::mutex mMutex;
    useconds_t mDelayMicroSecond = delayMicroSeconds;
    bool mIsThreadRunning{false};
};

}

#endif // SERVICEFACTORY_H

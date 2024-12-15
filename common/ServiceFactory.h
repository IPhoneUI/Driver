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
                usleep(10000);
            }
        });

        mServices.emplace(ins, msqReceiverThread);

        return ins;
    }

    void initialize();
    void finialize();
    
private:
    ServiceFactory();

    std::unordered_map<BaseServiceImpl*, std::thread*> mServices;
    std::shared_mutex mMutexProcess;

    std::mutex mMutex;
};

}

#endif // SERVICEFACTORY_H

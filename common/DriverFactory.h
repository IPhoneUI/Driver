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
#include <shared_mutex>

namespace driver::common {

class DriverFactory {
public:
    ~DriverFactory();
    static DriverFactory& instance();

    template <typename T>
    T *addDriver()
    {
        T* ins = new T();

        std::thread* threadExe = new std::thread([this, ins] 
        {
            while (true)
            {
                mMutexProcess.lock_shared();
                ins->onMsqReceived();
                mMutexProcess.unlock_shared();

                usleep(10000);
            }
        });

        mDrivers.emplace(ins, threadExe);

        return ins;
    }

    void initialize();
    void finialize();
    
private:
    DriverFactory();

    std::unordered_map<BaseDriver*, std::thread*> mDrivers;
    std::shared_mutex mMutexProcess;
};

}

#endif // DRIVERFACTORY_H

#ifndef DRIVERFACTORY_H
#define DRIVERFACTORY_H

#include <BaseDriver.h>
#include <vector>
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
        mDrivers.push_back(ins);

        // std::thread threadExe([this, ins] 
        // {
        //     while (mIsThreadRunning)
        //     {
        //         mMutexProcess.lock_shared();
        //         ins->execute();
        //         mMutexProcess.unlock_shared();

        //         usleep(10000);
        //     }
        // });
        // threadExe.detach();

        return ins;
    }

    void initialize();
    void finialize();
    
void execute();

private:
    DriverFactory();

    std::vector<BaseDriver*> mDrivers;
    std::thread mThread;
    bool mIsThreadRunning;
    std::shared_mutex mMutexProcess;
};

}

#endif // DRIVERFACTORY_H

#include "DriverExecution.h"
#include <utils/Logger.h>
#include "BaseDriver.h"

namespace common {

using time_clock = std::chrono::system_clock;

DriverExecution::DriverExecution()
    : mThread(std::bind(&DriverExecution::execute, this))
{
    mThread.detach();
}


DriverExecution::~DriverExecution()
{
    mIsThreadRunning = false;
}

DriverExecution &DriverExecution::instance()
{
    static DriverExecution ins;
    return ins;
}

void DriverExecution::addDriver(const std::string& clientName, BaseDriver *obj)
{
    if (obj == nullptr)
        return;

    std::lock_guard<std::mutex> lock(mMutex);
    auto it = mDrivers.begin();

    for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
    {
        if (it->first == clientName)
        {
            LOG_WARN("%s exists in driver list", clientName.c_str());
            return;
        }
    }
    
    obj->readDataFromDatabase();
    mDrivers.emplace(clientName, obj);
}

void DriverExecution::execute()
{
    mIsThreadRunning = true;
    static time_clock::time_point lastUpdate = time_clock::now();
    while (mIsThreadRunning)
    {
        milliseconds delta = std::chrono::duration_cast<milliseconds>(time_clock::now() - lastUpdate);
        lastUpdate = time_clock::now();

        mMutexProcess.lock_shared();
        auto it = mDrivers.begin();
        while (it != mDrivers.end())
        {
            if (it->second != nullptr)
            {
                it->second->execute(delta);
                it++;
            }
        }
        mMutexProcess.unlock_shared();

        usleep(mDelayMicroSecond);
    }
}

}

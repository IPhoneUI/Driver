#include "DriverFactory.h"
#include <utils/Logger.h>

namespace driver::common {

using time_clock = std::chrono::system_clock;

DriverFactory::DriverFactory()
    : mThread(std::bind(&DriverFactory::execute, this))
{
    mThread.detach();
}


DriverFactory::~DriverFactory()
{
    mIsThreadRunning = false;
}

DriverFactory &DriverFactory::instance()
{
    static DriverFactory ins;
    return ins;
}

void DriverFactory::initialize()
{
    for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
    {
        (*it).second->detach();
        (*it).first->initialize();
    }
}

void DriverFactory::finialize()
{
    for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
    {
        (*it).first->finialize();
    }
}

void DriverFactory::execute()
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
            if (it->first != nullptr)
            {
                it->first->execute(delta);
                it++;
            }
        }
        mMutexProcess.unlock_shared();

        usleep(mDelayMicroSecond);
    }
}

}

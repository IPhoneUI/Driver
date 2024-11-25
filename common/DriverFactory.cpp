#include "DriverFactory.h"
#include <utils/Logger.h>

static constexpr int16_t delayMicroSeconds = 10000; // 0.01s

namespace driver::common {

DriverFactory::DriverFactory()
    : mThread(std::bind(&DriverFactory::execute, this))
{
    mThread.detach();
}


DriverFactory::~DriverFactory()
{
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
        (*it)->initialize();
    }
}

void DriverFactory::finialize()
{
    for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
    {
        (*it)->finialize();
    }
}

void DriverFactory::execute()
{
    mIsThreadRunning = true;
    while (mIsThreadRunning)
    {
        mMutexProcess.lock_shared();
        for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
        {
            (*it)->execute();
        }
        mMutexProcess.unlock_shared();

        usleep(delayMicroSeconds);
    }
}

}

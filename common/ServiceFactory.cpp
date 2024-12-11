#include "ServiceFactory.h"
#include <utils/Logger.h>

namespace common {

using time_clock = std::chrono::system_clock;

ServiceFactory::ServiceFactory()
    : mThread(std::bind(&ServiceFactory::execute, this))
{
    mThread.detach();
}


ServiceFactory::~ServiceFactory()
{
}

ServiceFactory &ServiceFactory::instance()
{
    static ServiceFactory ins;
    return ins;
}

void ServiceFactory::initialize()
{
    for (auto it = mServices.begin(); it != mServices.end(); ++it)
    {
        (*it).second->detach();
        (*it).first->initialize();
    }
}

void ServiceFactory::finialize()
{
    for (auto it = mServices.begin(); it != mServices.end(); ++it)
    {
        (*it).first->finialize();
    }
}

void ServiceFactory::execute()
{
    mIsThreadRunning = true;
    static time_clock::time_point lastUpdate = time_clock::now();
    while (mIsThreadRunning)
    {
        milliseconds delta = std::chrono::duration_cast<milliseconds>(time_clock::now() - lastUpdate);
        lastUpdate = time_clock::now();

        mMutexProcess.lock_shared();
        auto it = mServices.begin();
        while (it != mServices.end())
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

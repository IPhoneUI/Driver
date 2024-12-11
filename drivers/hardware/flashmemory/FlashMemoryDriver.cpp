#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

static FlashMemoryDriver* gInstance = 0;

FlashMemoryDriver::FlashMemoryDriver()
{
    mProvider = base::shm::FlashMemoryProvider::instance();
}

FlashMemoryDriver* FlashMemoryDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new FlashMemoryDriver();
    }

    return gInstance;
}

void FlashMemoryDriver::initialize()
{
    LOG_INFO("FlashMemoryDriver initialize");
    if (gInstance == nullptr)
    {
        gInstance = new FlashMemoryDriver();
    }
}

void FlashMemoryDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void FlashMemoryDriver::requestChangeAirPlaneMode(bool airPlane)
{
    // auto result = mProvider->setAirPlaneMode(airPlane);

    // if (result == base::shm::DataSetResult_Valid)
    // {
    //     mDeploy->responseChangeAirPlaneMode(airPlane);
    // }
}

}
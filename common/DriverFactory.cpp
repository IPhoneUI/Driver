#include "DriverFactory.h"
#include <utils/Logger.h>

static constexpr int16_t delayMicroSeconds = 10000; // 0.01s

namespace driver::common {

DriverFactory::DriverFactory()
{
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

}

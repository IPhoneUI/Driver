#include "DriverFactory.h"

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

}

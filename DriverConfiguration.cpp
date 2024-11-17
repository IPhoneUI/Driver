#include "DriverConfiguration.h"
#include <DriverFactory.h>

namespace driver {

DriverConfiguration::DriverConfiguration()
{
    mSIMDriver = common::DriverFactory::instance().addDriver<SIMDriver>();
}

DriverConfiguration::~DriverConfiguration()
{
}

void DriverConfiguration::start()
{
    std::cout << "Start" << std::endl;
    common::DriverFactory::instance().initialize();
}

void DriverConfiguration::stop()
{
    std::cout << "Stop" << std::endl;
    common::DriverFactory::instance().finialize();
}

}
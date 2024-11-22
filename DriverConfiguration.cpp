#include "DriverConfiguration.h"
#include <DriverFactory.h>
#include <utils/Logger.h>

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
    LOG_INFO("DriverConfiguration Start");
    common::DriverFactory::instance().initialize();
}

void DriverConfiguration::stop()
{
    LOG_INFO("DriverConfiguration Stop");
    common::DriverFactory::instance().finialize();
}

}
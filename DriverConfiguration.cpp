#include "DriverConfiguration.h"
#include <DriverFactory.h>
#include <utils/Logger.h>

namespace driver {

DriverConfiguration::DriverConfiguration()
{
    common::DriverFactory::instance().addDriver<SIMDriver>();
    common::DriverFactory::instance().addDriver<SpeakerDriver>();
    common::DriverFactory::instance().addDriver<EasyMathServer>();
    common::DriverFactory::instance().addDriver<FlashMemoryDriver>();
    common::DriverFactory::instance().addDriver<WifiDriver>();
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
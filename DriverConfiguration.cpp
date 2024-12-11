#include "DriverConfiguration.h"
#include <ServiceFactory.h>
#include <utils/Logger.h>
#include <audio/AudioServiceImpl.h>
#include <cellularnetwork/CellularNetworkServiceImpl.h>
#include <easymath/EasyMathServiceImpl.h>
#include <phonebook/PhoneBookServiceImpl.h>
#include <pstn/PSTNServiceImpl.h>
#include <systemsetting/SystemSettingServiceImpl.h>
#include <wifi/WifiServiceImpl.h>
#include <flashmemory/FlashMemoryDriver.h>
#include <wifi/WifiDriver.h>
#include <speaker/SpeakerDriver.h>
#include <sim/SIMDriver.h>
#include <easymath/EasyMathServer.h>

namespace driver {

DriverConfiguration::DriverConfiguration()
{
    common::ServiceFactory::instance().addService<service::AudioServiceImpl>();
    common::ServiceFactory::instance().addService<service::CellularNetworkServiceImpl>();
    common::ServiceFactory::instance().addService<service::EasyMathServiceImpl>();
    common::ServiceFactory::instance().addService<service::PhoneBookServiceImpl>();
    common::ServiceFactory::instance().addService<service::PSTNServiceImpl>();
    common::ServiceFactory::instance().addService<service::SystemSettingServiceImpl>();
    common::ServiceFactory::instance().addService<service::WifiServiceImpl>();
}

DriverConfiguration::~DriverConfiguration()
{
}

void DriverConfiguration::start()
{
    LOG_INFO("DriverConfiguration Start");
    driver::EasyMathServer::initialize();
    driver::FlashMemoryDriver::initialize();
    driver::WifiDriver::initialize();
    driver::SIMDriver::initialize();
    driver::SpeakerDriver::initialize();

    common::ServiceFactory::instance().initialize();
}

void DriverConfiguration::stop()
{
    LOG_INFO("DriverConfiguration Stop");
    common::ServiceFactory::instance().finialize();
}

}
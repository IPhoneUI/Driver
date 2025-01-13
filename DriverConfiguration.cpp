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
#include <easymath/EasyMathServer.h>
#include <flashmemory/FlashMemoryDriver.h>
#include <wifi/WifiDriver.h>
#include <speaker/SpeakerDriver.h>
#include <sim/SIMDriver.h>
#include <DriverExecution.h>
#include <simulate/SimulateServer.h>
#include <weather/WeatherServiceImpl.h>
#include <WeatherDriver.h>

namespace driver {

DriverConfiguration::DriverConfiguration()
{
    common::SimulateServer::instance().start();
    initDriver();
    addService();
}

DriverConfiguration::~DriverConfiguration()
{
}

void DriverConfiguration::start()
{
    LOG_INFO("DriverConfiguration Start");
    common::ServiceFactory::instance().initialize();
}

void DriverConfiguration::stop()
{
    LOG_INFO("DriverConfiguration Stop");
    common::ServiceFactory::instance().finialize();
    common::DriverExecution::instance().finialize();
}

void DriverConfiguration::addService()
{
    common::ServiceFactory::instance().addService<service::AudioServiceImpl>();
    common::ServiceFactory::instance().addService<service::CellularNetworkServiceImpl>();
    common::ServiceFactory::instance().addService<service::EasyMathServiceImpl>();
    common::ServiceFactory::instance().addService<service::PhoneBookServiceImpl>();
    common::ServiceFactory::instance().addService<service::PSTNServiceImpl>();
    common::ServiceFactory::instance().addService<service::SystemSettingServiceImpl>();
    common::ServiceFactory::instance().addService<service::WifiServiceImpl>();
    common::ServiceFactory::instance().addService<service::WeatherServiceImpl>();
}

void DriverConfiguration::initDriver()
{
    EasyMathServer::initialize();
    FlashMemoryDriver::initialize();
    SpeakerDriver::initialize();
    SIMDriver::initialize();
    WifiDriver::initialize();
    WeatherDriver::initialize();
}

}

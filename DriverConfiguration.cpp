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
#include <datamanager/DataRepoManager.h>

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
    common::ServiceFactory::instance().initialize();
    common::DataRepoManager::instance().pull();
}

void DriverConfiguration::stop()
{
    LOG_INFO("DriverConfiguration Stop");
    common::ServiceFactory::instance().finialize();
    common::DataRepoManager::instance().push();
}

}

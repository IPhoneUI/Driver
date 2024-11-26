#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

FlashMemoryDriver::FlashMemoryDriver()
{
    mDeploy = FlashMemoryDeploy::instance();
    mProvider = FlashMemoryProvider::instance();
}

void FlashMemoryDriver::execute()
{
    std::vector<std::string> messages = mMqReceiver.receive("/flashmemoryReq");
    if (!messages.empty()) 
    {
        service::Msq_SystemSettingReq type = static_cast<service::Msq_SystemSettingReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case service::SysSett_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->registerClient(service::Msq_SysSettClient, clientName);
            break;
        }
        case service::SysSett_RequestSync: {
            mDeploy->requestChangeAirPlaneMode(mAirplaneMode);
            break;
        }
        case service::SysSett_RequestChangeAirplaneMode: {
            mAirplaneMode = mMqReceiver.get<bool>(messages[1]);
            mDeploy->requestChangeAirPlaneMode(mAirplaneMode);
            break;
        }
        }
    }
}

void FlashMemoryDriver::initialize()
{
    LOG_INFO("FlashMemoryDriver initialize");
}

void FlashMemoryDriver::finialize()
{
    LOG_INFO("FlashMemoryDriver finialize");
    mProvider->closeShmem();
}

}
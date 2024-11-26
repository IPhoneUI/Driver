#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

FlashMemoryDriver::FlashMemoryDriver()
{
    mFlMemoryDeploy = FlashMemoryDeploy::instance();
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
            mFlMemoryDeploy->registerClient(service::Msq_SysSettClient, clientName);
            break;
        }
        case service::SysSett_RequestSync: {
            mFlMemoryDeploy->requestChangeAirPlaneMode(mAirplaneMode);
            break;
        }
        case service::SysSett_RequestChangeAirplaneMode: {
            mAirplaneMode = static_cast<AirplaneMode>(mMqReceiver.get<int>(messages[1]));
            mFlMemoryDeploy->requestChangeAirPlaneMode(mAirplaneMode);
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
}

}
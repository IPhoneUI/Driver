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
        service::Msq_FlashMemoryReq type = static_cast<service::Msq_FlashMemoryReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case service::FMem_SysSett_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->registerClient(service::Msq_SysSett_Client, clientName);
            break;
        }
        case service::FMem_SysSett_ReqSync: {
            bool airplane = mProvider->getAirPlaneMode();
            mDeploy->requestChangeAirPlaneMode(airplane);
            break;
        }
        case service::FMem_SysSett_ReqChangeAirplaneMode: {
            bool airplane = mMqReceiver.get<bool>(messages[1]);
            mProvider->setAirPlaneMode(airplane);
            mDeploy->requestChangeAirPlaneMode(airplane);
            break;
        }
        // case service::FMem_Audio_RegisterClient: {
        //     std::string clientName = mMqReceiver.get<std::string>(messages[1]);
        //     // LOG_INFO("clientName: %s", clientName.c_str());
        //     // mDeploy->registerClient(service::Msq_Audio_Client, clientName);
        //     break;
        // }
        // case service::FMem_Audio_ReqSync: {
            // auto recordings = mProvider->getRecordingList();
            // mDeploy->updateRecordingList(recordings);
            // break;
        // }
        }
    }
}

void FlashMemoryDriver::initialize()
{
    LOG_INFO("FlashMemoryDriver initialize");
    mProvider->initialize();
}

void FlashMemoryDriver::finialize()
{
    LOG_INFO("FlashMemoryDriver finialize");
    mProvider->closeShmem();
}

}
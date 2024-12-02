#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

FlashMemoryDriver::FlashMemoryDriver()
{
    mDeploy = FlashMemoryDeploy::instance();
    mProvider = FlashMemoryProvider::instance();
}

void FlashMemoryDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/flashmemoryReq");
    if (!messages.empty()) 
    {
        service::Msq_FlashMemoryReq type = static_cast<service::Msq_FlashMemoryReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case service::FMem_SysSett_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(service::Msq_SysSett_Client, clientName);
            break;
        }
        case service::FMem_Audio_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(service::Msq_Audio_Client, clientName);
            break;
        }
        case service::FMem_SysSett_ReqSync: 
        case service::FMem_Audio_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
            break;
        }
        case service::FMem_SysSett_ReqChangeAirplaneMode: {
            bool airplane = mMqReceiver.get<bool>(messages[1]);
            requestChangeAirPlaneMode(airplane);
            break;
        }
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

void FlashMemoryDriver::registerClient(service::Msq_Client clientId, const std::string& clientName)
{
    if (mDeploy->registerClient(clientId, clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void FlashMemoryDriver::requestSync(service::Msq_FlashMemoryReq type, const std::string& clientName)
{
    if (type == service::FMem_SysSett_ReqSync)
    {
        mDeploy->responseSyncSysSetting(clientName);

    }
    else if (type == service::FMem_Audio_ReqSync)
    {
        mDeploy->responseSyncAudio(clientName);
    }
}

void FlashMemoryDriver::requestChangeAirPlaneMode(bool airPlane)
{
    auto result = mProvider->setAirPlaneMode(airPlane);

    if (result == DataSetResult_Valid)
    {
        mDeploy->responseChangeAirPlaneMode(airPlane);
    }
}

}
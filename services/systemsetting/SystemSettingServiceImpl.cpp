#include "SystemSettingServiceImpl.h"
#include <utils/Logger.h>

namespace service {

SystemSettingServiceImpl::SystemSettingServiceImpl()
{
    mDeploy = SystemSettingServiceDeploy::instance();
    mFlmemProvider = base::shm::FlashMemoryProvider::instance();
}

void SystemSettingServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/sysSettingReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_SysSett_Req type = static_cast<base::msq::Msq_SysSett_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_SysSett_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_SysSett_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        case base::msq::Msq_SysSett_ReqChangeAirPlaneMode: {
            bool airplane = mMqReceiver.get<bool>(messages[1]);
            requestChangeAirPlaneMode(airplane);
            break;
        }
        }
    }
}

void SystemSettingServiceImpl::initialize()
{
    LOG_INFO("SystemSettingServiceImpl initialize");
    mFlmemProvider->initialize();
}

void SystemSettingServiceImpl::finialize()
{
    LOG_INFO("SystemSettingServiceImpl finialize");
    mFlmemProvider->closeShmem();
}

void SystemSettingServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void SystemSettingServiceImpl::requestChangeAirPlaneMode(bool airPlane)
{
    auto result = mFlmemProvider->setAirPlaneMode(airPlane);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeAirPlaneMode(airPlane);
    }
}

}
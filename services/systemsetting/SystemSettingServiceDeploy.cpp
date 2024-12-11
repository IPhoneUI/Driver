#include "SystemSettingServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static SystemSettingServiceDeploy* gInstance = nullptr;

SystemSettingServiceDeploy::SystemSettingServiceDeploy()
{
}

SystemSettingServiceDeploy::~SystemSettingServiceDeploy()
{
}

SystemSettingServiceDeploy* SystemSettingServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SystemSettingServiceDeploy();
    }
    return gInstance;
}

void SystemSettingServiceDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_SysSett_RespReady);
    mMqSender.sendMsq(clientName);
}

void SystemSettingServiceDeploy::responseSync(const std::string& clientName)
{
    base::shm::FlashMemoryProvider* provider = base::shm::FlashMemoryProvider::instance();

    std::lock_guard<std::mutex> lock(mMutex);
    bool airplane = provider->getAirPlaneMode();
    mMqSender.startMsq(base::msq::Msq_SysSett_RespAirPlaneModeUpdated);
    mMqSender.addParam(airplane);
    mMqSender.sendMsq(clientName);
}

void SystemSettingServiceDeploy::responseChangeAirPlaneMode(bool airPlane)
{
    mClientManager.deploy([this, airPlane](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_SysSett_RespAirPlaneModeUpdated);
        mMqSender.addParam(airPlane);
        mMqSender.sendMsq(mqName);
    });
}


}

#include "FlashMemoryDeploy.h"
#include <utils/Logger.h>

namespace driver {

static FlashMemoryDeploy* gInstance = nullptr;

FlashMemoryDeploy::FlashMemoryDeploy()
{
}

FlashMemoryDeploy* FlashMemoryDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new FlashMemoryDeploy();
    }
    return gInstance;
}

void FlashMemoryDeploy::requestChangeAirPlaneMode(const AirplaneMode& airPlane)
{
    LOG_INFO("FlashMemoryDeploy requestChangeAirPlaneMode");
    mClientManager.deploy(service::Msq_SysSettClient, [this, airPlane](std::string mqName) {
        mMqSender.startMsq(service::FlashMem_ChangeAirplaneMode);
        mMqSender.addParam(static_cast<int>(airPlane));
        mMqSender.sendMsq(mqName);
    });
}

}
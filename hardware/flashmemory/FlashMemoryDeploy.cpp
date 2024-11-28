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

void FlashMemoryDeploy::requestChangeAirPlaneMode(bool airPlane)
{
    LOG_INFO("FlashMemoryDeploy requestChangeAirPlaneMode");
    mClientManager.execute(service::Msq_SysSettClient, [this, airPlane](std::string mqName) {
        mMqSender.startMsq(service::FlashMem_ChangeAirplaneMode);
        mMqSender.addParam(airPlane);
        mMqSender.sendMsq(mqName);
    });
}

}
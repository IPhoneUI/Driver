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
    mMqSender.startMsq(service::SysSett_ResponseChangeAirplaneMode);
    mMqSender.addParam(static_cast<int>(airPlane));
    mMqSender.sendMsq("/flashmemoryDep_", service::Msq_ReqAll);
    
}

}
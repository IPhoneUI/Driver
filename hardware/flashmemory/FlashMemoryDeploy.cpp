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

void FlashMemoryDeploy::syncSysSetting(const AirplaneMode& airPlain)
{
    LOG_INFO("BAO syncSysSetting");
    mMqSender.startMsq(static_cast<int>(airPlain));
    mMqSender.sendMsq("/flashmemory_", service::Msq_ReqAll);
    
}

}
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
    mClientManager.execute(service::Msq_SysSett_Client, [this, airPlane](std::string mqName) {
        mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
        mMqSender.addParam(airPlane);
        mMqSender.sendMsq(mqName);
    });
}

void FlashMemoryDeploy::updateRecordingList(const std::list<service::VoiceRecordingData>& list)
{
    mClientManager.execute(service::Msq_Audio_Client, [this, list](std::string mqName) {
        // mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
        // mMqSender.addParam(airPlane);
        // mMqSender.sendMsq(mqName);
    });
}

}
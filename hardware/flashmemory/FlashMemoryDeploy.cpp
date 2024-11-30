#include "FlashMemoryDeploy.h"
#include <utils/Logger.h>
#include <flashmemory/FlashMemoryProvider.h>

namespace driver {

static FlashMemoryDeploy* gInstance = nullptr;

FlashMemoryDeploy::FlashMemoryDeploy()
{
}

FlashMemoryDeploy::~FlashMemoryDeploy()
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

void FlashMemoryDeploy::syncSysSetting(const std::string& clientName)
{
    FlashMemoryProvider* provider = FlashMemoryProvider::instance();

    bool airplane = provider->getAirPlaneMode();
    mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
    mMqSender.addParam(airplane);
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::syncAudio(const std::string& clientName)
{
    mMqSender.startMsq(service::FMem_Audio_RespUpdateRecording);
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::responseChangeAirPlaneMode(bool airPlane)
{
    mClientManager.execute(service::Msq_SysSett_Client, [this, airPlane](std::string mqName) {
        mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
        mMqSender.addParam(airPlane);
        mMqSender.sendMsq(mqName);
    });
}

void FlashMemoryDeploy::responseUpdateRecordingList()
{
    mClientManager.execute(service::Msq_Audio_Client, [this](std::string mqName) {
        mMqSender.startMsq(service::FMem_Audio_RespUpdateRecording);
        mMqSender.sendMsq(mqName);
    });
}

}
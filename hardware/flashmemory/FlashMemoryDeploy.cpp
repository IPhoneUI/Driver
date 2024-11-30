#include "FlashMemoryDeploy.h"
#include <utils/Logger.h>
#include <flashmemory/FlashMemoryProvider.h>

namespace driver {

static FlashMemoryDeploy* gInstance = nullptr;

FlashMemoryDeploy::FlashMemoryDeploy()
    : common::BaseDeploy("FlashMemoryDeploy")
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

void FlashMemoryDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(service::FMem_Ready);
    mMqSender.addParam(mDriverName.c_str());
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::syncSysSetting(const std::string& clientName)
{
    FlashMemoryProvider* provider = FlashMemoryProvider::instance();

    std::lock_guard<std::mutex> lock(mMutex);
    bool airplane = provider->getAirPlaneMode();
    mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
    mMqSender.addParam(mDriverName.c_str());
    mMqSender.addParam(airplane);
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::syncAudio(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::FMem_Audio_RespUpdateRecording);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::FMem_Audio_RespUpdateDelRecording);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.sendMsq(clientName);
    }
}

void FlashMemoryDeploy::responseChangeAirPlaneMode(bool airPlane)
{
    mClientManager.execute(service::Msq_SysSett_Client, [this, airPlane](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::FMem_SysSett_RespChangeAirplaneMode);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.addParam(airPlane);
        mMqSender.sendMsq(mqName);
    });
}

void FlashMemoryDeploy::responseUpdateRecordingList()
{
    mClientManager.execute(service::Msq_Audio_Client, [this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::FMem_Audio_RespUpdateRecording);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.sendMsq(mqName);
    });
}

}
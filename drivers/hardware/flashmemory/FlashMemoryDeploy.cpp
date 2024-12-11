#include "FlashMemoryDeploy.h"
#include <utils/Logger.h>
#include <flashmemory/FlashMemoryProvider.h>

namespace driver {

static FlashMemoryDeploy* gInstance = nullptr;

FlashMemoryDeploy::FlashMemoryDeploy()
    // : common::BaseDeploy(base::msq::Msq_FMem_Type)
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
    mMqSender.startMsq(base::msq::FMem_Ready);
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::responseSyncSysSetting(const std::string& clientName)
{
    base::shm::FlashMemoryProvider* provider = base::shm::FlashMemoryProvider::instance();

    std::lock_guard<std::mutex> lock(mMutex);
    bool airplane = provider->getAirPlaneMode();
    mMqSender.startMsq(base::msq::FMem_SysSett_RespChangeAirplaneMode);
    mMqSender.addParam(airplane);
    mMqSender.sendMsq(clientName);
}

void FlashMemoryDeploy::responseSyncAudio(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::FMem_Audio_RespUpdateRecording);
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::FMem_Audio_RespUpdateDelRecording);
        mMqSender.sendMsq(clientName);
    }
}

void FlashMemoryDeploy::responseChangeAirPlaneMode(bool airPlane)
{
    // mClientManager.deploy(base::msq::Msq_SysSett_Client, [this, airPlane](std::string mqName) {
    //     std::lock_guard<std::mutex> lock(mMutex);
    //     mMqSender.startMsq(base::msq::FMem_SysSett_RespChangeAirplaneMode);
    //     mMqSender.addParam(mDriverType);
    //     mMqSender.addParam(airPlane);
    //     mMqSender.sendMsq(mqName);
    // });
}

void FlashMemoryDeploy::responseUpdateRecordingList()
{
    // mClientManager.deploy(base::msq::Msq_Audio_Client, [this](std::string mqName) {
    //     std::lock_guard<std::mutex> lock(mMutex);
    //     mMqSender.startMsq(base::msq::FMem_Audio_RespUpdateRecording);
    //     mMqSender.addParam(mDriverType);
    //     mMqSender.sendMsq(mqName);
    // });
}

}
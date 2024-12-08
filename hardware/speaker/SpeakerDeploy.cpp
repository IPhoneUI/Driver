#include "SpeakerDeploy.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDeploy* gInstance = nullptr;

SpeakerDeploy::SpeakerDeploy()
    : common::BaseDeploy(base::msq::Msq_Speaker_Type)
{
    mProvider = base::shm::SpeakerProvider::instance();
}

SpeakerDeploy::~SpeakerDeploy()
{
}

SpeakerDeploy* SpeakerDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SpeakerDeploy();
    }
    return gInstance;
}

void SpeakerDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Speaker_Ready);
    mMqSender.addParam(mDriverType);
    mMqSender.sendMsq(clientName);
}

void SpeakerDeploy::responseSyncAudio(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool isMute = mProvider->getIsMuted();
        mMqSender.startMsq(base::msq::Speaker_Audio_RespChangeMute);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(isMute);
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        int volume = mProvider->getVolume();
        mMqSender.startMsq(base::msq::Speaker_Audio_RespChangeVolume);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(volume);
        mMqSender.sendMsq(clientName);
    }
}

void SpeakerDeploy::responseAudioMute(const bool &status)
{
    mClientManager.execute(base::msq::Msq_Audio_Client, [this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Speaker_Audio_RespChangeMute);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

}

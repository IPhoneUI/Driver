#include "SpeakerDeploy.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDeploy* gInstance = nullptr;

SpeakerDeploy::SpeakerDeploy()
    : common::BaseDeploy("SpeakerDeploy")
{
    mProvider = SpeakerProvider::instance();
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
    mMqSender.startMsq(service::Speaker_Ready);
    mMqSender.addParam(mDriverName.c_str());
    mMqSender.sendMsq(clientName);
}

void SpeakerDeploy::syncAudio(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool isMute = mProvider->getIsMuted();
        mMqSender.startMsq(service::Speaker_Audio_RespMute);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.addParam(isMute);
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        int volume = mProvider->getVolume();
        mMqSender.startMsq(service::Speaker_Audio_RespVolume);
        mMqSender.addParam(mDriverName.c_str());
        mMqSender.addParam(volume);
        mMqSender.sendMsq(clientName);
    }
}

}
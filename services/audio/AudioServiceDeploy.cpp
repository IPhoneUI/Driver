#include "AudioServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static AudioServiceDeploy* gInstance = nullptr;

AudioServiceDeploy::AudioServiceDeploy()
{
    mSpeakerProvider = base::shm::SpeakerProvider::instance();
}

AudioServiceDeploy::~AudioServiceDeploy()
{
}

AudioServiceDeploy* AudioServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new AudioServiceDeploy();
    }
    return gInstance;
}

void AudioServiceDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_Audio_RespReady);
    mMqSender.sendMsq(clientName);
}

void AudioServiceDeploy::responseSync(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespRecordingUpdated);
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespDelRecordingUpdated);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool isMute = mSpeakerProvider->getIsMuted();
        mMqSender.startMsq(base::msq::Msq_Audio_RespMuteUpdated);
        mMqSender.addParam(isMute);
        mMqSender.sendMsq(clientName);
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        int volume = mSpeakerProvider->getVolume();
        mMqSender.startMsq(base::msq::Msq_Audio_RespVolumeUpdated);
        mMqSender.addParam(volume);
        mMqSender.sendMsq(clientName);
    }
}


}

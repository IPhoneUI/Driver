#include "AudioServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static AudioServiceDeploy* gInstance = nullptr;

AudioServiceDeploy::AudioServiceDeploy()
{
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

void AudioServiceDeploy::responseServiceReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_Audio_RespReady);
    mMqSender.sendMsq(clientName);
}

void AudioServiceDeploy::responseVolumeUpdated(int volume)
{
    mClientManager.deploy([this, volume](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespVolumeUpdated);
        mMqSender.addParam(volume);
        mMqSender.sendMsq(mqName);
    });
}

void AudioServiceDeploy::responseMuteUpdated(bool isMuted)
{
    mClientManager.deploy([this, isMuted](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespMuteUpdated);
        mMqSender.addParam(isMuted);
        mMqSender.sendMsq(mqName);
    });
}

void AudioServiceDeploy::responseRecordingDataUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespRecordingUpdated);
        mMqSender.sendMsq(mqName);
    });
}

void AudioServiceDeploy::responseDeleteRecordingDataUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Audio_RespDelRecordingUpdated);
        mMqSender.sendMsq(mqName);
    });
}

}

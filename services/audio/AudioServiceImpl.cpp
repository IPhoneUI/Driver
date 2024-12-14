#include "AudioServiceImpl.h"
#include <utils/Logger.h>

namespace service {

AudioServiceImpl::AudioServiceImpl()
{
    mDeploy = AudioServiceDeploy::instance();
    mSpeakerDriver = driver::SpeakerDriver::getInstance();
    mFMemDriver = driver::FlashMemoryDriver::getInstance();
    mAudioProvider = base::shm::AudioProvider::instance();
}

void AudioServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/audioServiceReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_Audio_Req type = static_cast<base::msq::Msq_Audio_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_Audio_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_Audio_ReqSync: {
            {
                bool isMuted = mSpeakerDriver->getIsMuted();
                mDeploy->responseMuteUpdated(isMuted);

                int volume = mSpeakerDriver->getVolume();
                mDeploy->responseVolumeUpdated(volume);
            }
            {
                auto recordingList = mFMemDriver->getVoiceRecordingList();
                onVoiceRecordingDataUpdated(recordingList);

                auto deleteRecordingList = mFMemDriver->getVoiceDeleteRecordingList();
                onVoiceDeleteRecordingDataUpdated(deleteRecordingList);
            }
            break;
        }
        case base::msq::Msq_Audio_ReqChangeMute: {
            bool mute = mMqReceiver.get<bool>(messages[1]);
            mSpeakerDriver->requestChangeAudioMute(mute);
        }
        }
    }
}

void AudioServiceImpl::initialize()
{
    LOG_INFO("AudioServiceImpl initialize");
    Connection::connect(mSpeakerDriver->onDriverReady, std::bind(&AudioServiceImpl::onSpeakerDriverReady, this));
    Connection::connect(mSpeakerDriver->onVolumeUpdated, std::bind(&AudioServiceImpl::onVolumeUpdated, this, std::placeholders::_1));
    Connection::connect(mSpeakerDriver->onMuteUpdated, std::bind(&AudioServiceImpl::onMuteUpdated, this, std::placeholders::_1));
    
    Connection::connect(mFMemDriver->onDriverReady, std::bind(&AudioServiceImpl::onFMemDriverReady, this));
    Connection::connect(mFMemDriver->onVoiceRecordingDataUpdated, std::bind(&AudioServiceImpl::onVoiceRecordingDataUpdated, this, std::placeholders::_1));
    Connection::connect(mFMemDriver->onVoiceDeleteRecordingDataUpdated, std::bind(&AudioServiceImpl::onVoiceDeleteRecordingDataUpdated, this, std::placeholders::_1));

    mSpeakerDriver->connectDriver();
    mFMemDriver->connectDriver();
}

void AudioServiceImpl::finialize()
{
    LOG_INFO("AudioServiceImpl finialize");
}

void AudioServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

void AudioServiceImpl::onSpeakerDriverReady()
{
    // To do
}

void AudioServiceImpl::onVolumeUpdated(int volume)
{
    mDeploy->responseVolumeUpdated(volume);
}

void AudioServiceImpl::onMuteUpdated(bool mute)
{
    mDeploy->responseMuteUpdated(mute);
}

void AudioServiceImpl::onFMemDriverReady()
{
    // To do
}

void AudioServiceImpl::onVoiceRecordingDataUpdated(std::list<service::VoiceRecordingData*> data)
{
    LOG_INFO("onVoiceRecordingDataUpdated size: %d", data.size());
    if (mAudioProvider->setRecordingList(data))
    {
        mDeploy->responseRecordingDataUpdated();
    }
}

void AudioServiceImpl::onVoiceDeleteRecordingDataUpdated(std::list<service::VoiceRecordingData*> data)
{
    LOG_INFO("onVoiceDeleteRecordingDataUpdated size: %d", data.size());
    if (mAudioProvider->setDeleteRecordingList(data))
    {
        mDeploy->responseDeleteRecordingDataUpdated();
    }
}

}
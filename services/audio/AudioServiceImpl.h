#ifndef AUDIOSERVICEIMPL_H
#define AUDIOSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "AudioServiceDeploy.h"
#include <speaker/SpeakerDriver.h>
#include <flashmemory/FlashMemoryDriver.h>
#include <audio/AudioProvider.h>

namespace service {

class AudioServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit AudioServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName) override;

public:
    void onSpeakerDriverReady();
    void onVolumeUpdated(int);
    void onMuteUpdated(bool);

public:
    void onFMemDriverReady();
    void onVoiceRecordingDataUpdated(std::list<service::VoiceRecordingData*>);
    void onVoiceDeleteRecordingDataUpdated(std::list<service::VoiceRecordingData*>);

private:
    AudioServiceDeploy* mDeploy;
    driver::SpeakerDriver* mSpeakerDriver;
    driver::FlashMemoryDriver* mFMemDriver;

    base::shm::AudioProvider* mAudioProvider;
};

}

#endif //AUDIOSERVICEIMPL_H
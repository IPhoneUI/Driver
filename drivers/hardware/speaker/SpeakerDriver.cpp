#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDriver* gInstance = 0;

SpeakerDriver::SpeakerDriver()
{
    mRepo.setName("speaker");
    mRepo.addParam("muted", common::ParameterIndex::Speaker_Muted);
    mRepo.addParam("volume", common::ParameterIndex::Speaker_Volume);

    Connection::connect(mRepo.onRepoStateChanged, std::bind(&SpeakerDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepo.pull();

    common::DriverExecution::instance().addDriver("SpeakerDriver", this);
}

SpeakerDriver* SpeakerDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The SpeakerDriver has not been initialized yet");
    }

    return gInstance;
}

void SpeakerDriver::initialize()
{
    if (gInstance == nullptr)
    {
        gInstance = new SpeakerDriver();
    }
}

void SpeakerDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void SpeakerDriver::writeBuffer()
{
    mRepo[common::ParameterIndex::Speaker_Muted] = mIsMuted;
    mRepo[common::ParameterIndex::Speaker_Volume] = mVolume;

    mRepo.push();
}

void SpeakerDriver::onSimulateReceived(const std::string& topic, const std::string& option, const std::string& content)
{
    if (topic == "HardKey")
    {
        if (option == "Volume")
        {
            if (content == "Increase")
            {
                mVolume += 1;
                onVolumeUpdated.emit(mVolume);
            }
            else if (content == "Decrease")
            {
                mVolume -= 1;
                onVolumeUpdated.emit(mVolume);
            }
        }
        else if (option == "Mute")
        {
            if (content == "ON" && !mIsMuted)
            {
                mIsMuted = true;
                onMuteUpdated.emit(mIsMuted);
            }
            else if (content == "OFF" && mIsMuted)
            {
                mIsMuted = false;
                onMuteUpdated.emit(mIsMuted);
            }
        }
    }
}

void SpeakerDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mIsMuted = mRepo[common::ParameterIndex::Speaker_Muted];
        mVolume = mRepo[common::ParameterIndex::Speaker_Volume];
    }
}

void SpeakerDriver::changeAudioMute(bool isMuted)
{
    if (mIsMuted == isMuted)
        return;
    
    mIsMuted = isMuted;
    onMuteUpdated.emit(mIsMuted);
}

}

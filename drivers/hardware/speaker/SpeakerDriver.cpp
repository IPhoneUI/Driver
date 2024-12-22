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

void SpeakerDriver::writeData()
{
    mIsMuted = mRepo[common::ParameterIndex::Speaker_Muted] = mIsMuted;
    mVolume = mRepo[common::ParameterIndex::Speaker_Volume] = mVolume;
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

#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDriver* gInstance = 0;

SpeakerDriver::SpeakerDriver()
{
    common::DriverExecution::instance().addDriver("SpeakerDriver", this);
}

SpeakerDriver* SpeakerDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SpeakerDriver();
    }

    return gInstance;
}

void SpeakerDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void SpeakerDriver::readDataFromDatabase()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    if (dataRepo.isReady())
    {
        common::Repository& repo = dataRepo.getRepository("speaker");
        mIsMuted = repo[common::ParameterIndex::Speaker_Muted];
        mVolume = repo[common::ParameterIndex::Speaker_Volume];
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

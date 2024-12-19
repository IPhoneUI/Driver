#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDriver* gInstance = 0;

SpeakerDriver::SpeakerDriver()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    auto speakerRepo = dataRepo.addRepository("speaker");
    if (speakerRepo != nullptr)
    {
        speakerRepo->addParameter("muted", common::ParameterIndex::Speaker_Muted);
        speakerRepo->addParameter("volume", common::ParameterIndex::Speaker_Volume);

        Connection::connect(speakerRepo->onRepoStateChanged, std::bind(&SpeakerDriver::onRepoStateChanged, this, std::placeholders::_1));
    }
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
}

void SpeakerDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
        common::Repository& repo = dataRepo.repository("speaker");

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

#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

static SpeakerDriver* gInstance = 0;

SpeakerDriver::SpeakerDriver()
{
    mProvider = base::shm::SpeakerProvider::instance();
}

SpeakerDriver* SpeakerDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SpeakerDriver();
    }

    return gInstance;
}

void SpeakerDriver::initialize()
{
    LOG_INFO("SpeakerDriver initialize");
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
// void SpeakerDriver::requestChangeAudioMute(const bool &status)
// {
//     if (mProvider->getIsMuted() != status) {
//         base::shm::DataSetResult result = mProvider->setAudioMute(status);

//         if (result == base::shm::DataSetResult_Valid) {
//             mDeploy->responseAudioMute(status);
//         }
//     }
// }

}

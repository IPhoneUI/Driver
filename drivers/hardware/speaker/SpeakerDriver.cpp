#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

SpeakerDriver::SpeakerDriver()
{
    mDeploy = SpeakerDeploy::instance();
    mProvider = base::shm::SpeakerProvider::instance();
}

void SpeakerDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/speakerReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_SpeakerReq type = static_cast<base::msq::Msq_SpeakerReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Speaker_Audio_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(base::msq::Msq_Audio_Client, clientName);
            break;
        }
        case base::msq::Speaker_Audio_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
            break;
        }
        case base::msq::Speaker_Audio_ReqChangeMute: {
            bool isMute = mMqReceiver.get<bool>(messages[1]);
            requestChangeAudioMute(isMute);
            break;
        }
        }
    }
}

void SpeakerDriver::initialize()
{
    LOG_INFO("SpeakerDriver initialize");
    mProvider->initialize();
}

void SpeakerDriver::finialize()
{
    LOG_INFO("SpeakerDriver finialize");
    mProvider->closeShmem();
}

void SpeakerDriver::registerClient(base::msq::Msq_Client clientId, const std::string& clientName)
{
    // if (mDeploy->registerClient(clientId, clientName))
    // {
    //     mDeploy->responseDriverReady(clientName);
    // }
}

void SpeakerDriver::requestSync(base::msq::Msq_SpeakerReq type, const std::string& clientName)
{
    mDeploy->responseSyncAudio(clientName);
}

void SpeakerDriver::requestChangeAudioMute(const bool &status)
{
    if (mProvider->getIsMuted() != status) {
        base::shm::DataSetResult result = mProvider->setAudioMute(status);

        if (result == base::shm::DataSetResult_Valid) {
            mDeploy->responseAudioMute(status);
        }
    }
}

}

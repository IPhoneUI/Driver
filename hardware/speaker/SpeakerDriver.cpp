#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

SpeakerDriver::SpeakerDriver()
{
    mDeploy = SpeakerDeploy::instance();
    mProvider = SpeakerProvider::instance();
}

void SpeakerDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/speakerReq");
    if (!messages.empty()) 
    {
        service::Msq_SpeakerReq type = static_cast<service::Msq_SpeakerReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case service::Speaker_Audio_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(service::Msq_Audio_Client, clientName);
            break;
        }
        case service::Speaker_Audio_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
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

void SpeakerDriver::registerClient(service::Msq_Client clientId, const std::string& clientName)
{
    if (mDeploy->registerClient(clientId, clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void SpeakerDriver::requestSync(service::Msq_SpeakerReq type, const std::string& clientName)
{
    mDeploy->responseSyncAudio(clientName);
}

}
#include "AudioServiceImpl.h"
#include <utils/Logger.h>

namespace service {

AudioServiceImpl::AudioServiceImpl()
{
    mDeploy = AudioServiceDeploy::instance();
    mFlmemProvider = base::shm::FlashMemoryProvider::instance();
    mSpeakerProvider = base::shm::SpeakerProvider::instance();
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
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        }
    }
}

void AudioServiceImpl::initialize()
{
    LOG_INFO("AudioServiceImpl initialize");
    mFlmemProvider->initialize();
    mSpeakerProvider->initialize();
}

void AudioServiceImpl::finialize()
{
    LOG_INFO("AudioServiceImpl finialize");
    mFlmemProvider->closeShmem();
    mSpeakerProvider->closeShmem();
}

void AudioServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

}
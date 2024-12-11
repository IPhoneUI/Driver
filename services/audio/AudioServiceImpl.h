#ifndef AUDIOSERVICEIMPL_H
#define AUDIOSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "AudioServiceDeploy.h"
#include <flashmemory/FlashMemoryProvider.h>
#include <speaker/SpeakerProvider.h>

namespace service {

class AudioServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit AudioServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName) override;

private:
    AudioServiceDeploy* mDeploy;
    base::shm::FlashMemoryProvider* mFlmemProvider;
    base::shm::SpeakerProvider* mSpeakerProvider;
};

}

#endif //AUDIOSERVICEIMPL_H
#ifndef SPEAKERDRIVER_H
#define SPEAKERDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include "SpeakerDeploy.h"
#include <speaker/SpeakerProvider.h>

namespace driver {

class SpeakerDriver : public common::BaseDriver 
{
public:
    explicit SpeakerDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);
    void requestSync(base::msq::Msq_SpeakerReq type, const std::string& clientName);

    void requestChangeAudioMute(const bool& status);
    
private:    
    SpeakerDeploy* mDeploy;
    base::shm::SpeakerProvider* mProvider;
};

}

#endif //SPEAKERDRIVER_H

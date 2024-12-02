#ifndef SPEAKERDEPLOY_H
#define SPEAKERDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>
#include <speaker/SpeakerProvider.h>

namespace driver {

class SpeakerDeploy : public common::BaseDeploy
{
public:
    static SpeakerDeploy* instance();

    void responseDriverReady(const std::string& clientName);
    void responseSyncAudio(const std::string& clientName);

private:
    explicit SpeakerDeploy();
    ~SpeakerDeploy();

    SpeakerProvider* mProvider;
    std::mutex mMutex;
};

}

#endif //SPEAKERDEPLOY_H
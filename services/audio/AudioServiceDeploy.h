#ifndef AUDIODSERVICEEPLOY_H
#define AUDIODSERVICEEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <speaker/SpeakerProvider.h>

namespace service {

class AudioServiceDeploy : public common::BaseDeploy
{
public:
    static AudioServiceDeploy* instance();

    void responseDriverReady(const std::string& clientName);
    void responseSync(const std::string& clientName);

private:
    explicit AudioServiceDeploy();
    ~AudioServiceDeploy();

    std::mutex mMutex;
    base::shm::SpeakerProvider* mSpeakerProvider;
};

}

#endif //AUDIODSERVICEEPLOY_H

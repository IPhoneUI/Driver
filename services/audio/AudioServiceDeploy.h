#ifndef AUDIODSERVICEEPLOY_H
#define AUDIODSERVICEEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>

namespace service {

class AudioServiceDeploy : public common::BaseDeploy
{
public:
    static AudioServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseVolumeUpdated(int volume);
    void responseMuteUpdated(bool isMuted);

    void responseRecordingDataUpdated();
    void responseDeleteRecordingDataUpdated();

private:
    explicit AudioServiceDeploy();
    ~AudioServiceDeploy();

    std::mutex mMutex;
};

}

#endif //AUDIODSERVICEEPLOY_H

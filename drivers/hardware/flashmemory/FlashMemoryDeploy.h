#ifndef FLASHMEMORYDEPLOY_H
#define FLASHMEMORYDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>

namespace driver {

class FlashMemoryDeploy : public common::BaseDeploy
{
public:
    static FlashMemoryDeploy* instance();

    void responseDriverReady(const std::string& clientName);
    void responseSyncSysSetting(const std::string& clientName);
    void responseSyncAudio(const std::string& clientName);
    void responseChangeAirPlaneMode(bool airPlane);
    void responseUpdateRecordingList();

private:
    explicit FlashMemoryDeploy();
    ~FlashMemoryDeploy();

    std::mutex mMutex;
};

}

#endif //FLASHMEMORYDEPLOY_H
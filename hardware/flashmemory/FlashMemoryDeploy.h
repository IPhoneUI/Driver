#ifndef FLASHMEMORYDEPLOY_H
#define FLASHMEMORYDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>

namespace driver {

class FlashMemoryDeploy : public common::BaseDeploy
{
public:
    static FlashMemoryDeploy* instance();

    void syncSysSetting(const std::string& clientName);
    void syncAudio(const std::string& clientName);
    void responseChangeAirPlaneMode(bool airPlane);
    void responseUpdateRecordingList();

private:
    explicit FlashMemoryDeploy();
    ~FlashMemoryDeploy();
};

}

#endif //FLASHMEMORYDEPLOY_H
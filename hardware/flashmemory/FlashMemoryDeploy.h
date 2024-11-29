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

    void requestChangeAirPlaneMode(bool airPlane);
    void updateRecordingList(const std::list<service::VoiceRecordingData>& list);

private:
    explicit FlashMemoryDeploy();
};

}

#endif //FLASHMEMORYDEPLOY_H
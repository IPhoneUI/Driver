#ifndef FLASHMEMORYDRIVER_H
#define FLASHMEMORYDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include "FlashMemoryDef.h"
#include "FlashMemoryDeploy.h"

namespace driver {

class FlashMemoryDriver : public common::BaseDriver 
{
public:
    explicit FlashMemoryDriver();

    void execute() override;
    void initialize() override;
    void finialize() override;

    // std::list <VoiceRecordingData> getRecordingList() const
    // {
    //     return mRecordings;
    // }

    // std::list <VoiceRecordingData> getDeletedRecording() const
    // {
    //     return mDeletedRecording;
    // }

private:
    // std::list<VoiceRecordingData> mRecordings;
    // std::list<VoiceRecordingData> mDeletedRecording;
    AirplaneMode mAirplaneMode = AirplaneMode::Inactive;
    FlashMemoryDeploy* mFlMemoryDeploy;
};

}

#endif //FLASHMEMORYDRIVER_H
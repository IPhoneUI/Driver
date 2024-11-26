#ifndef FLASHMEMORYDRIVER_H
#define FLASHMEMORYDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include "FlashMemoryDeploy.h"
#include <flashmemory/FlashMemoryProvider.h>

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
    bool mAirplaneMode = false;
    FlashMemoryDeploy* mDeploy;
    FlashMemoryProvider* mProvider;
};

}

#endif //FLASHMEMORYDRIVER_H
#ifndef FLASHMEMORYDRIVER_H
#define FLASHMEMORYDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <audio/AudioServiceDef.h>

namespace driver {

class FlashMemoryDriver : public common::BaseDriver 
{
public:
    static FlashMemoryDriver* getInstance();
    static void initialize();

    void connectDriver() override;

    void changeAirPlaneMode(bool airPlane);

    std::list<service::VoiceRecordingData*> getVoiceRecordingList() const
    {
        return mRecordingData;
    }

    std::list<service::VoiceRecordingData*> getVoiceDeleteRecordingList() const
    {
        return mDeleteRecordingData;
    }

    bool getAirPlaneMode() const
    {
        return mAirPlaneMode;
    }

    Signal<std::list<service::VoiceRecordingData*>> onVoiceRecordingDataUpdated;
    Signal<std::list<service::VoiceRecordingData*>> onVoiceDeleteRecordingDataUpdated;
    Signal<bool> onAirPlaneModeUpdated;

private: 
    void onRepoStateChanged(common::Repository::State state);

private:
    explicit FlashMemoryDriver();

    std::list<service::VoiceRecordingData*> mRecordingData;
    std::list<service::VoiceRecordingData*> mDeleteRecordingData;
    bool mAirPlaneMode;
};

}

#endif //FLASHMEMORYDRIVER_H
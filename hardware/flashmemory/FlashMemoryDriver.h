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

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(service::Msq_Client clientId, const std::string& clientName);
    void requestSync(service::Msq_FlashMemoryReq type, const std::string& clientName);
    void requestChangeAirPlaneMode(bool airPlane);

    // std::list <VoiceRecordingData> getDeletedRecording() const
    // {
    //     return mDeletedRecording;
    // }

private:
    // std::list<VoiceRecordingData> mDeletedRecording;
    FlashMemoryDeploy* mDeploy;
    FlashMemoryProvider* mProvider;
};

}

#endif //FLASHMEMORYDRIVER_H
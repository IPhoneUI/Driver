#ifndef PSTNSERVICEIMPL_H
#define PSTNSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <sim/SIMProvider.h>
#include <event/EventQueue.h>
#include <pstn/PSTNServiceDef.h>
#include "PSTNServiceDeploy.h"

namespace service {

class PSTNServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit PSTNServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(const std::string& clientName) override;
    void callNumber(const std::string& number);
    void answerCall();
    void rejectCall();
    void terminateCall();


private:
    enum class PSTNEvent {
        CallStatusUpdated,
        TimeUpdated,
        CallInfoUpdated
    };

    void updateTimeCall(milliseconds delta);

private:
    base::shm::SIMProvider* mSIMProvider;
    PSTNServiceDeploy* mDeploy;

    service::CallInformation* mCallInfo;
    base::event::EventQueue<PSTNEvent> mEventQueue;
};

}

#endif //PSTNSERVICEIMPL_H
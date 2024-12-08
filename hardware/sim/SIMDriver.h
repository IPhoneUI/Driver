#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <sim/SIMProvider.h>
#include <pstn/PSTNServiceDef.h>
#include "SIMDeploy.h"
#include <event/EventQueue.h>

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:
    explicit SIMDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);
    void requestSync(base::msq::Msq_SIMReq type, const std::string& clientName);

    void requestChangeCellularStatus(bool status);
    void requestChangeAllowAccess(bool status);
    void requestChangeMaxCompatibility(bool status);

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
    base::shm::SIMProvider* mProvider;
    SIMDeploy* mDeploy;

    service::CallInformation* mCallInfo;
    base::event::EventQueue<PSTNEvent> mEventQueue;
};

}

#endif //SIMDRIVER_H
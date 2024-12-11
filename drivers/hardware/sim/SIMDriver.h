#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <sim/SIMProvider.h>
#include <pstn/PSTNServiceDef.h>
#include <event/EventQueue.h>

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:
    static SIMDriver* getInstance();
    static void initialize();
    
    void execute(milliseconds delta) override;

    // void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);
    // void requestSync(base::msq::Msq_SIMReq type, const std::string& clientName);

    // void requestChangeCellularStatus(bool status);
    // void requestChangeAllowAccess(bool status);
    // void requestChangeMaxCompatibility(bool status);

    void connectDriver() override;
    void callNumber(const std::string& number);
    void answerCall();
    void rejectCall();
    void terminateCall();

    Signal<service::CallStatus, const std::string&> onCallStatusUpdated;
    Signal<const std::string&, const std::string&, const std::string&> onCallInfoUpdated;
    Signal<int> onTimeUpdated;

private:
    enum class PSTNEvent {
        CallStatusUpdated,
        TimeUpdated,
        CallInfoUpdated
    };

    explicit SIMDriver();
    void updateTimeCall(milliseconds delta);

private:
    service::CallInformation* mCallInfo;
    base::event::EventQueue<PSTNEvent> mEventQueue;
};

}

#endif //SIMDRIVER_H
#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <string>
#include <list>
#include <common/BaseDriver.h>
#include <sim/SIMProvider.h>
#include <pstn/PSTNServiceDef.h>
#include <event/EventQueue.h>
#include <phonebook/PhoneBookServiceDef.h>

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:
    enum ServiceReq {
        PhoneBookService,
        PSTNService
    };

    static SIMDriver* getInstance();
    static void initialize();
    
    void execute(milliseconds delta) override;
    void readDataFromDatabase();

    // void requestChangeCellularStatus(bool status);
    // void requestChangeAllowAccess(bool status);
    // void requestChangeMaxCompatibility(bool status);

    void requestSync(ServiceReq type);
    void connectDriver() override;
    void callNumber(const std::string& number);
    void answerCall();
    void rejectCall();
    void terminateCall();

    std::list<service::PhoneContactInfo> getContactList() const
    {
        return mContacts;
    }

    std::list<service::PhoneHistoryInfo> getHistoryList() const
    {
        return mHistories;
    }

    Signal<service::CallStatus, const std::string&> onCallStatusUpdated;
    Signal<const std::string&, const std::string&, const std::string&> onCallInfoUpdated;
    Signal<int> onTimeUpdated;
    Signal<> onPhoneContactListUpdated;
    Signal<> onPhoneHistoryListUpdated;

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

    std::string mPhoneNumber;
    std::string mNetwork;
    int mPhoneSignal;
    std::string mWifiPassword;
    bool mAllowAccess;
    bool mCellularSts;
    bool mMaxCompatibility;
    std::list<service::PhoneContactInfo> mContacts;
    std::list<service::PhoneHistoryInfo> mHistories;
};

}

#endif //SIMDRIVER_H
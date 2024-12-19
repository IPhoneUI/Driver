#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <string>
#include <list>
#include <common/BaseDriver.h>
#include <pstn/PSTNServiceDef.h>
#include <event/EventQueue.h>
#include <phonebook/PhoneBookServiceDef.h>

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:

    static SIMDriver* getInstance();
    
    void execute(milliseconds delta) override;
    void readDataFromDatabase() override;

    void changeCellularStatus(bool status);
    void changeAllowAccess(bool status);
    void changeMaxCompatibility(bool status);

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

    bool getAllowAccess() const
    {
        return mAllowAccess;
    }

    bool getCellularStatus() const
    {
        return mCellularSts;
    }

    bool getMaxCompatibitily() const
    {
        return mMaxCompatibility;
    }

    Signal<service::CallStatus, const std::string&> onCallStatusUpdated;
    Signal<const std::string&, const std::string&, const std::string&> onCallInfoUpdated;
    Signal<int> onTimeUpdated;
    Signal<> onPhoneContactListUpdated;
    Signal<> onPhoneHistoryListUpdated;

    Signal<bool> onAllowAccessUpdated;
    Signal<bool> onCellularStatusUpdated;
    Signal<bool> onMaxCompatibilityUpdated;

private:
    enum class PSTNEvent {
        CallStatusUpdated,
        TimeUpdated,
        CallInfoUpdated
    };

    explicit SIMDriver();
    void updateTimeCall(milliseconds delta);
    void onRepoStateChanged(common::Repository::State state);

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
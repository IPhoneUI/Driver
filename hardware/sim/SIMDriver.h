#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:
    explicit SIMDriver();

    void execute() override;
    void initialize() override;
    void finialize() override;

    enum PhoneSignal {
        Weak = 0,
        Media,
        Strong
    };

    enum ConnectState {
        Disconnected = 0,
        Connecting,
        Connected
    };

    bool cellularStatus() const
    {
        return mCellular;
    }

    bool maxCompatibility() const
    {
        return mMaxCompatibility;
    }

    bool allowAccessStatus() const
    {
        return mAllowAccess;
    }

    std::string wifiPassord() const
    {
        return mWifiPassword;
    }

    // std::list<service::PhoneContactInfo> getContactData() const
    // {
    //     return mContactData;
    // }

    // std::list<service::PhoneHistoryInfo> getHistoryData() const
    // {
    //     return mHistoryData;
    // }

    void updatePhoneNumber(const std::string& value);
    void updateNetwork(const std::string& value);
    void updatePhoneSignal(const PhoneSignal& value);
    void updateWifiPassword(const std::string& value);
    void updateCellularStatus(const bool& status);
    void updateAllowAccessStatus(const bool& status);
    void updateMaxCompatibility(const bool& status);

private:

    std::string mPhoneNumber;
    std::string mNetwork;
    PhoneSignal mPhoneSignal;
    std::string mWifiPassword;
    bool mAllowAccess;
    bool mCellular;
    bool mMaxCompatibility;
    ConnectState mState;

    // std::list<service::PhoneContactInfo> mContactData;
    // std::list<service::PhoneHistoryInfo> mHistoryData;
};

}

#endif //SIMDRIVER_H
#ifndef SIMDEPLOY_H
#define SIMDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>
#include <sim/SIMProvider.h>
#include <pstn/PSTNServiceDef.h>

namespace driver {

class SIMDeploy : public common::BaseDeploy
{
public:
    static SIMDeploy* instance();

    // Cellular Network
    void responseSyncCelNetwork(const std::string& clientName);
    void responseDriverReady(const std::string& clientName);
    void responseChangeCellularStatus(bool status);
    void responseChangeAllowAccess(bool status);
    void responseChangeMaxCompatibility(bool status);

    // PhoneBook
    void responseSyncPhoneBook(const std::string& clientName);

    // PSTN
    void responseCallStatusUpdated(service::CallStatus status, const std::string& number);
    void responseCallInfoUpdated(const std::string& number, const std::string& name, const std::string& avatar);
    void responseTimeUpdated(int time);

private:
    explicit SIMDeploy();
    ~SIMDeploy();

    SIMProvider* mProvider;
    std::mutex mMutex;
};

}

#endif //SIMDEPLOY_H
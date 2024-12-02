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

namespace driver {

class SIMDeploy : public common::BaseDeploy
{
public:
    static SIMDeploy* instance();

    void responseSyncCelNetwork(const std::string& clientName);
    void responseDriverReady(const std::string& clientName);
    void responseChangeCellularStatus(bool status);
    void responseChangeAllowAccess(bool status);
    void responseChangeMaxCompatibility(bool status);

    void responseSyncPhoneBook(const std::string& clientName);

private:
    explicit SIMDeploy();
    ~SIMDeploy();

    SIMProvider* mProvider;
    std::mutex mMutex;
};

}

#endif //SIMDEPLOY_H
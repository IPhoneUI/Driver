#ifndef SYSTEMSETTINGSERVICEDEPLOY_H
#define SYSTEMSETTINGSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <flashmemory/FlashMemoryProvider.h>

namespace service {

class SystemSettingServiceDeploy : public common::BaseDeploy
{
public:
    static SystemSettingServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseSync(const std::string& clientName);
    void responseChangeAirPlaneMode(bool airPlane);

private:
    explicit SystemSettingServiceDeploy();
    ~SystemSettingServiceDeploy();

    std::mutex mMutex;
};

}

#endif //SYSTEMSETTINGSERVICEDEPLOY_H

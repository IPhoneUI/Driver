#ifndef PSTNSERVICEDEPLOY_H
#define PSTNSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <pstn/PSTNServiceDef.h>

namespace service {

class PSTNServiceDeploy : public common::BaseDeploy
{
public:
    static PSTNServiceDeploy* instance();
    void responseServiceReady(const std::string& clientName) override;
    void responseCallStatusUpdated(service::CallStatus status, const std::string& number);
    void responseCallInfoUpdated(const std::string& number, const std::string& name, const std::string& avatar);
    void responseTimeUpdated(int time);

private:
    explicit PSTNServiceDeploy();
    ~PSTNServiceDeploy();

    std::mutex mMutex;
};

}

#endif //PSTNSERVICEDEPLOY_H

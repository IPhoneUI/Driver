#ifndef PSTNSERVICEIMPL_H
#define PSTNSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <sim/SIMProvider.h>
#include <event/EventQueue.h>
#include <pstn/PSTNServiceDef.h>
#include <sim/SIMDriver.h>
#include "PSTNServiceDeploy.h"
#include <Connection.h>

namespace service {

class PSTNServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit PSTNServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(const std::string& clientName) override;

private:
    void onDriverReady();
    void onTimeUpdated(int);
    void onCallStatusUpdated(service::CallStatus, const std::string&);
    void onCallInfoUpdated(const std::string&, const std::string&, const std::string&);
private:
    // base::shm::SIMProvider* mSIMProvider;
    driver::SIMDriver* mSIMDriver;
    PSTNServiceDeploy* mDeploy;
};

}

#endif //PSTNSERVICEIMPL_H
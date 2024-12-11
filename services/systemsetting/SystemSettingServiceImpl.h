#ifndef SYSTEMSETTINGSERVICEIMPL_H
#define SYSTEMSETTINGSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <flashmemory/FlashMemoryProvider.h>
#include "SystemSettingServiceDeploy.h"

namespace service {

class SystemSettingServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit SystemSettingServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(const std::string& clientName) override;
    void requestChangeAirPlaneMode(bool airPlane);
    
private:
    SystemSettingServiceDeploy* mDeploy;
    base::shm::FlashMemoryProvider* mFlmemProvider;
};

}

#endif //SYSTEMSETTINGSERVICEIMPL_H
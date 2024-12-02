#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <sim/SIMProvider.h>
#include "SIMDeploy.h"

namespace driver {

class SIMDriver : public common::BaseDriver 
{
public:
    explicit SIMDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(service::Msq_Client clientId, const std::string& clientName);
    void requestSync(service::Msq_SIMReq type, const std::string& clientName);

    void requestChangeCellularStatus(bool status);
    void requestChangeAllowAccess(bool status);
    void requestChangeMaxCompatibility(bool status);

private:
    SIMProvider* mProvider;
    SIMDeploy* mDeploy;
};

}

#endif //SIMDRIVER_H
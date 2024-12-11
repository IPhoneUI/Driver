#ifndef CELLULARNETWORKSERVICEIMPL_H
#define CELLULARNETWORKSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <sim/SIMProvider.h>
#include "CellularNetworkServiceDeploy.h"

namespace service {

class CellularNetworkServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit CellularNetworkServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName) override;

    void requestChangeCellularStatus(bool status);
    void requestChangeAllowAccess(bool status);
    void requestChangeMaxCompatibility(bool status);

private:
    base::shm::SIMProvider* mSIMProvider;
    CellularNetworkServiceDeploy* mDeploy;
};

}

#endif //CELLULARNETWORKSERVICEIMPL_H
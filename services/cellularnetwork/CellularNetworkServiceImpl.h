#ifndef CELLULARNETWORKSERVICEIMPL_H
#define CELLULARNETWORKSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "CellularNetworkServiceDeploy.h"
#include <sim/SIMDriver.h>

namespace service {

class CellularNetworkServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit CellularNetworkServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName) override;

public:
    void onSIMDriverReady();
    void onAllowAccessUpdated(bool);
    void onMaxCompatibilityUpdated(bool);
    void onCellularStatusUpdated(bool);

private:
    CellularNetworkServiceDeploy* mDeploy;

    driver::SIMDriver* mSIMDriver;
};

}

#endif //CELLULARNETWORKSERVICEIMPL_H
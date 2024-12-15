#ifndef CELLULARNETWORKSERVICEDEPLOY_H
#define CELLULARNETWORKSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>

namespace service {

class CellularNetworkServiceDeploy : public common::BaseDeploy
{
public:
    static CellularNetworkServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseChangeCellularStatus(bool status);
    void responseChangeAllowAccess(bool status);
    void responseChangeMaxCompatibility(bool status);

private:
    explicit CellularNetworkServiceDeploy();
    ~CellularNetworkServiceDeploy();

    std::mutex mMutex;
};

}

#endif //CELLULARNETWORKSERVICEDEPLOY_H

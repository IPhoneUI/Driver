#ifndef BASEDEPLOY_H
#define BASEDEPLOY_H

#include <queue>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <vector>
#include <msq/MsqSender.h>
#include <ClientManager.h>

namespace driver::common {

class BaseDeploy 
{
public:
    BaseDeploy(const std::string& driverName);
    bool registerClient(service::Msq_Client type, const std::string& clientName);

protected:
    std::string mDriverName;
    service::MsqSender mMqSender;
    ClientManager mClientManager;
};

}


#endif
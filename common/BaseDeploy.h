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
#include <msq/MsqDef.h>

namespace driver::common {

class BaseDeploy 
{
public:
    BaseDeploy(service::Msq_Driver_Type driverType);
    bool registerClient(service::Msq_Client type, const std::string& clientName);

protected:
    service::Msq_Driver_Type mDriverType;
    service::MsqSender mMqSender;
    ClientManager mClientManager;
};

}


#endif
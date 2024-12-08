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
    BaseDeploy(base::msq::Msq_Driver_Type driverType);
    bool registerClient(base::msq::Msq_Client type, const std::string& clientName);

protected:
    base::msq::Msq_Driver_Type mDriverType;
    base::msq::MsqSender mMqSender;
    ClientManager mClientManager;
};

}


#endif
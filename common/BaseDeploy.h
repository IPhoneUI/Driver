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
#include "DriverExecution.h"

namespace common {

class BaseDeploy 
{
public:
    bool registerClient(const std::string& clientName);
    virtual void responseServiceReady(const std::string& clientName) = 0;

protected:
    base::msq::MsqSender mMqSender;
    ClientManager mClientManager;
};

}


#endif
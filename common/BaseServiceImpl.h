#ifndef BASESERVICEIMPL_H
#define BASESERVICEIMPL_H

#include <queue>
#include <string>
#include <mutex>
#include <chrono>
#include <vector>
#include <shared_mutex>
#include <functional>
#include <unordered_map>
#include <msq/MsqReceiver.h>
#include <msq/MsqDef.h>
#include "BaseDeploy.h"

namespace common {

#define milliseconds std::chrono::milliseconds

class BaseServiceImpl 
{
public:
    BaseServiceImpl(BaseDeploy* deploy)
        : mDeploy(deploy)
    {}

    BaseServiceImpl()
    {}
    
    virtual void initialize() = 0;
    virtual void finialize() = 0;
    virtual void onMsqReceived() = 0;
    virtual void registerClient(const std::string& clientName) = 0;

protected:
    base::msq::MsqReceiver mMqReceiver;
    BaseDeploy* mDeploy;
    std::vector<std::string> mClientWaitReady;
};

}


#endif
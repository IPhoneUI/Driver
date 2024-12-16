#include "BaseServiceImpl.h"

namespace common {

BaseServiceImpl::BaseServiceImpl(BaseDeploy* deploy)
    : mDeploy(deploy)
{
}

BaseServiceImpl::BaseServiceImpl()
{
}

void BaseServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

}
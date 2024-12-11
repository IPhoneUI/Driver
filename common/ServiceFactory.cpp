#include "ServiceFactory.h"
#include <utils/Logger.h>

namespace common {

ServiceFactory::ServiceFactory()
{
}


ServiceFactory::~ServiceFactory()
{
}

ServiceFactory &ServiceFactory::instance()
{
    static ServiceFactory ins;
    return ins;
}

void ServiceFactory::initialize()
{
    for (auto it = mServices.begin(); it != mServices.end(); ++it)
    {
        (*it).second->detach();
        (*it).first->initialize();
    }
}

void ServiceFactory::finialize()
{
    for (auto it = mServices.begin(); it != mServices.end(); ++it)
    {
        (*it).first->finialize();
    }
}

}

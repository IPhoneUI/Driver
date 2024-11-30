#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

BaseDeploy::BaseDeploy(const std::string& driverName)
    : mDriverName(driverName)
{
    
}

bool BaseDeploy::registerClient(service::Msq_Client type, const std::string& clientName)
{
    return mClientManager.registerGroup(type, clientName);
}

}
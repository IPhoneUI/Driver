#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

BaseDeploy::BaseDeploy(base::msq::Msq_Driver_Type driverType)
    : mDriverType(driverType)
{
    
}

bool BaseDeploy::registerClient(base::msq::Msq_Client type, const std::string& clientName)
{
    return mClientManager.registerGroup(type, clientName);
}

}
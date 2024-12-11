#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace common {

bool BaseDeploy::registerClient(const std::string& clientName)
{
    return mClientManager.registerClient(clientName);
}

}
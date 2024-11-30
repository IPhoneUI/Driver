#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

bool BaseDeploy::registerClient(service::Msq_Client type, const std::string& clientName)
{
    return mClientManager.registerGroup(type, clientName);
}

void BaseDeploy::responseDriverReady(const std::string& clientName)
{
    mMqSender.startMsq(service::FMem_Ready);
    mMqSender.sendMsq(clientName);
}

}
#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

void BaseDeploy::registerClient(service::Msq_Client type, const std::string& clientName)
{
    mMqSender.startMsq(service::FlashMem_Ready);
    mMqSender.sendMsq(clientName);
    mClientManager.registerGroup(type, clientName);

}

}
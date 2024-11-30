#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

void BaseDeploy::registerClient(service::Msq_Client type, const std::string& clientName)
{
    LOG_INFO("Deploy clientName: %s", clientName.c_str());
    switch (type) {
    case service::Msq_SysSett_Client:
        mMqSender.startMsq(service::FMem_SysSett_Ready);
        break;
    case service::Msq_Audio_Client:
        mMqSender.startMsq(service::FMem_Audio_Ready);
        break;
    }
    mMqSender.sendMsq(clientName);
    mClientManager.registerGroup(type, clientName);

}

}
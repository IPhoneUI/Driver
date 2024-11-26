#include "BaseDeploy.h"
#include <utils/Logger.h>

namespace driver::common {

void BaseDeploy::addClient(const std::string& clientName)
{
    bool exist = false;
    for (const auto& client : mClients)
    {
        if (client == clientName)
        {
            LOG_WARN("clientName is exists");
            return;
        }
    }

    mClients.push_back(clientName);
    mMqSender.startMsq(service::FlashMem_Ready);
    mMqSender.sendMsq(clientName);
}

}
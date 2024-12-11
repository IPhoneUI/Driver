#include "ClientManager.h"
#include <utils/Logger.h>

namespace common {

bool ClientManager::registerClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (std::find(mClients.begin(), mClients.end(), client) != mClients.end())
        return false;

    mClients.push_back(client);
    return true;
}

bool ClientManager::removeClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mClients.erase(std::remove(mClients.begin(), mClients.end(), client), mClients.end());

    return true;
}

void ClientManager::deploy(std::function<void(std::string)> func)
{
    if (func == nullptr)
        return;

    for (auto client : mClients)
    {
        func(client);
    }
}

}
#include "ClientManager.h"
#include <utils/Logger.h>

namespace driver {

void ClientManager::registerGroup(service::Msq_Client type, const std::string& clientName)
{
    ClientGroup* group = nullptr;
    {
        std::lock_guard<std::mutex> lock(mMutex);
        auto it = mGroups.find(type);

        if (it == mGroups.end())
        {
            group = new ClientGroup();
            mGroups[type] = group;
        }
        else 
        {
            group = it->second;
        }
    }
    if (group != nullptr)
    {
        group->addClient(clientName);
    }
}

void ClientManager::deploy(service::Msq_Client type, std::function<void(std::string)> func)
{
    auto it = mGroups.find(type);
    if (it == mGroups.end())
    {
        LOG_WARN("Can not deploy with invalid type");
        return;
    }

    if (mGroups[type] != nullptr)
    {
        mGroups[type]->deploy(func);
    }
}

void ClientManager::ClientGroup::addClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mGroupMutex);
    if (std::find(mClients.begin(), mClients.end(), client) != mClients.end())
        return;

    mClients.push_back(client);
}

void ClientManager::ClientGroup::removeClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mGroupMutex);
    mClients.erase(std::remove(mClients.begin(), mClients.end(), client), mClients.end());
}

void ClientManager::ClientGroup::deploy(std::function<void(std::string)> func)
{
    if (func == nullptr)
        return;

    for (auto client : mClients)
    {
        func(client);
    }
}

}
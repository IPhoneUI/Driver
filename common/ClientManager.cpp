#include "ClientManager.h"
#include <utils/Logger.h>

namespace driver {

bool ClientManager::registerGroup(service::Msq_Client type, const std::string& clientName)
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
        return group->addClient(clientName);
    }
    
    return false;
}

void ClientManager::execute(service::Msq_Client type, std::function<void(std::string)> func)
{
    auto it = mGroups.find(type);
    if (it == mGroups.end())
    {
        LOG_WARN("Can not deploy with invalid type");
        return;
    }

    if (mGroups[type] != nullptr)
    {
        mGroups[type]->execute(func);
    }
}

bool ClientManager::ClientGroup::addClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mGroupMutex);
    if (std::find(mClients.begin(), mClients.end(), client) != mClients.end())
        return false;

    mClients.push_back(client);
    return true;
}

void ClientManager::ClientGroup::removeClient(const std::string& client)
{
    std::lock_guard<std::mutex> lock(mGroupMutex);
    mClients.erase(std::remove(mClients.begin(), mClients.end(), client), mClients.end());
}

void ClientManager::ClientGroup::execute(std::function<void(std::string)> func)
{
    if (func == nullptr)
        return;

    for (auto client : mClients)
    {
        func(client);
    }
}

}
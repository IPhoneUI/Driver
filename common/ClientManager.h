#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <msq/MsqDef.h>
#include <mutex>
#include <shared_mutex>

namespace driver {

class ClientManager 
{
public:
    class ClientGroup 
    {
    public:
        void addClient(const std::string& client);
        void removeClient(const std::string& client);
        void execute(std::function<void(std::string)> func);
    
    private: 
        std::mutex mGroupMutex;
        std::vector<std::string> mClients;
    };

    void registerGroup(service::Msq_Client type, const std::string& clientName);
    void execute(service::Msq_Client type, std::function<void(std::string)> func);

private:
    std::mutex mMutex;
    std::unordered_map<service::Msq_Client, ClientGroup*>  mGroups;
};

}


#endif // CLIENTMANAGER_H
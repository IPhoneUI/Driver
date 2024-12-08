#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <msq/MsqDef.h>
#include <mutex>
#include <shared_mutex>

namespace driver::common {

class ClientManager 
{
public:
    class ClientGroup 
    {
    public:
        bool addClient(const std::string& client);
        void removeClient(const std::string& client);
        void execute(std::function<void(std::string)> func);
    
    private: 
        std::mutex mGroupMutex;
        std::vector<std::string> mClients;
    };

    bool registerGroup(base::msq::Msq_Client type, const std::string& clientName);
    void execute(base::msq::Msq_Client type, std::function<void(std::string)> func);

private:
    std::mutex mMutex;
    std::unordered_map<base::msq::Msq_Client, ClientGroup*>  mGroups;
};

}


#endif // CLIENTMANAGER_H
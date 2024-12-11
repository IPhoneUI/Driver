#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <msq/MsqDef.h>
#include <mutex>
#include <shared_mutex>

namespace common {

class ClientManager 
{
public:
    bool registerClient(const std::string& clientName);
    bool removeClient(const std::string& client);
    void deploy(std::function<void(std::string)> func);

private:
    std::mutex mMutex;
    std::vector<std::string> mClients;
};

}


#endif // CLIENTMANAGER_H
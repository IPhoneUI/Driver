#ifndef BASEDEPLOY_H
#define BASEDEPLOY_H

#include <queue>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <vector>
#include <msq/MsqSender.h>

namespace driver::common {

class BaseDeploy 
{
public:
    void addClient(const std::string& clientName);

protected:
    service::MsqSender mMqSender;
    std::vector<std::string> mClients;
};

}


#endif
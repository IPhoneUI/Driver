#ifndef BASEDEPLOY_H
#define BASEDEPLOY_H

#include <queue>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <unordered_map>
#include <msq/MsqSender.h>

namespace driver::common {

class BaseDeploy 
{
public:

protected:
    service::MsqSender mMqSender;
};

}


#endif
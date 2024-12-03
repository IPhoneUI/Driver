#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <queue>
#include <string>
#include <mutex>
#include <chrono>
#include <shared_mutex>
#include <functional>
#include <unordered_map>
#include <msq/MsqReceiver.h>
#include <msq/MsqDef.h>


namespace driver::common {

#define milliseconds std::chrono::milliseconds

class BaseDriver 
{
public:
    virtual void initialize() = 0;
    virtual void finialize() = 0;
    virtual void execute(milliseconds delta);

    virtual void onMsqReceived() = 0;

protected:
    service::MsqReceiver mMqReceiver;
};

}


#endif
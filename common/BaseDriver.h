#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <string>
#include <Connection.h>
#include <datamanager/Repository.h>
#include <DriverExecution.h>
#include <simulate/SimulateServer.h>

namespace common {

#define milliseconds std::chrono::milliseconds

class BaseDriver 
{
public:
    virtual void connectDriver() = 0;
    virtual void execute(milliseconds delta);
    virtual void writeBuffer() = 0;
    virtual void onSimulateReceived(const std::string& topic, const std::string& option, const std::string& content) = 0;

    Signal<> onDriverReady;

protected:
    bool mIsReady {false};
};

}


#endif

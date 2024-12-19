#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <string>
#include <Connection.h>
#include <datamanager/DataRepoManager.h>
#include <DriverExecution.h>

namespace common {

#define milliseconds std::chrono::milliseconds

class BaseDriver 
{
public:
    virtual void connectDriver() = 0;
    virtual void execute(milliseconds delta);

    Signal<> onDriverReady;

protected:
    bool mIsReady {false};
};

}


#endif
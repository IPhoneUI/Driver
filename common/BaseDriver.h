#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <string>
#include <Connection.h>
#include <datamanager/Repository.h>
#include <DriverExecution.h>

namespace common {

#define milliseconds std::chrono::milliseconds

class BaseDriver 
{
public:
    virtual void connectDriver() = 0;
    virtual void execute(milliseconds delta);
    virtual void writeBuffer() = 0;

    Signal<> onDriverReady;

protected:
    bool mIsReady {false};
};

}


#endif
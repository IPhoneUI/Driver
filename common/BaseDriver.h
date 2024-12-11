#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <string>
#include <Connection.h>

namespace common {

#define milliseconds std::chrono::milliseconds

class BaseDriver 
{
public:
    virtual void execute(milliseconds delta);
    virtual void connectDriver() = 0;

    Signal<> onDriverReady;

protected:
    bool mIsReady {false};
};

}


#endif
#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

#include <queue>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <unordered_map>

namespace driver::common {

class AbstractDriver 
{
public:
    virtual void initialize() = 0;
    virtual void finialize() = 0;

protected:
};

}


#endif
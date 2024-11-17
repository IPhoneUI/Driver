#ifndef SIMDRIVER_H
#define SIMDRIVER_H

#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <AbstractDriver.h>

namespace driver {

class SIMDriver : public common::AbstractDriver 
{
public:
    explicit SIMDriver();

    void execute();
    void initialize() override;
    void finialize() override;

private:
    std::thread mThread;
};

}

#endif //SIMDRIVER_H
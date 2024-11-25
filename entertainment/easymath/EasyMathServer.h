#ifndef EASYMATHSERVER_H
#define EASYMATHSERVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>

namespace driver {

class EasyMathServer : public common::BaseDriver 
{
public:
    explicit EasyMathServer();

    void execute();
    void initialize() override;
    void finialize() override;

private:
    std::thread mThread;
};

}

#endif //EASYMATHSERVER_H
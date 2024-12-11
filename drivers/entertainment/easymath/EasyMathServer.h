#ifndef EASYMATHSERVER_H
#define EASYMATHSERVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>

namespace driver {

class EasyMathServer : public common::BaseServiceImpl 
{
public:
    explicit EasyMathServer();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

private:
};

}

#endif //EASYMATHSERVER_H
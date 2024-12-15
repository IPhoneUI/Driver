#ifndef EASYMATHSERVICEIMPL_H
#define EASYMATHSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "EasyMathServiceDeploy.h"
#include <easymath/EasyMathServer.h>

namespace service {

class EasyMathServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit EasyMathServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(const std::string& clientName);
    void onStartGame(const bool& isStart);

private:
    EasyMathServiceDeploy* mDeploy;
    driver::EasyMathServer* mEasyMathServer;
};

}

#endif //EASYMATHSERVICEIMPL_H

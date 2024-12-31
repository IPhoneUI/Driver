#ifndef EASYMATHSERVICEIMPL_H
#define EASYMATHSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "EasyMathServiceDeploy.h"
#include <easymath/EasyMathServer.h>
#include <utility>
#include <chrono>
#include <cmath>

namespace service {

class EasyMathServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit EasyMathServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName);

    void onHighestScoreChanged();
    void onStartGame();
    void onGameOver();
    void onScoreChanged(int score);
    void onExpressionChanged(const service::ExpressionInfo& info);

private:
    EasyMathServiceDeploy* mDeploy;
    driver::EasyMathServer* mEasyMathServer;
};

}

#endif //EASYMATHSERVICEIMPL_H

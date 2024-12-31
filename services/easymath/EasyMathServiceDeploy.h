#ifndef EASYMATHSERVICEDEPLOY_H
#define EASYMATHSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>
#include <easymath/EasyMathDef.h>

namespace service {

class EasyMathServiceDeploy : public common::BaseDeploy
{
public:
    static EasyMathServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseStartGame();
    void responseExpressionChanged(const service::ExpressionInfo& info);
    void responseGameOver(int score);
    void responseTimeIntervalUpdated(size_t time);

private:
    explicit EasyMathServiceDeploy();
    ~EasyMathServiceDeploy();

    std::mutex mMutex;
};

}

#endif //EASYMATHSERVICEDEPLOY_H

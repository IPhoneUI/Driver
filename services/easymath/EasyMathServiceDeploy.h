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
    void responseStartGame(const bool& status);
    void responseExpressionChanged(const service::ExpressionInfo& info);
    void responseScore(const int& score);

private:
    explicit EasyMathServiceDeploy();
    ~EasyMathServiceDeploy();

    std::mutex mMutex;
};

}

#endif //EASYMATHSERVICEDEPLOY_H

#include "EasyMathServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static EasyMathServiceDeploy* gInstance = nullptr;

EasyMathServiceDeploy::EasyMathServiceDeploy()
{
}

EasyMathServiceDeploy::~EasyMathServiceDeploy()
{
}

EasyMathServiceDeploy* EasyMathServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new EasyMathServiceDeploy();
    }
    return gInstance;
}

void EasyMathServiceDeploy::responseServiceReady(const std::string &clientName)
{
    LOG_INFO("THAIVD --- EasyMathServiceDeploy::responseServiceReady");
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::EasyMath_RespServiceReady);
    mMqSender.sendMsq(clientName);
}

void EasyMathServiceDeploy::responseStartGame(const bool &status)
{
    mClientManager.deploy([this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::EasyMath_RespStartGame);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

void EasyMathServiceDeploy::responseExpressionChanged(const ExpressionInfo &info)
{
    mClientManager.deploy([this, info](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::EasyMath_RespExpressionChange);
        mMqSender.addParam(info.firstNumber);
        mMqSender.addParam(info.secondNumber);
        mMqSender.addParam(static_cast<int>(info.exprType));
        mMqSender.addParam(info.dummyResult);
        mMqSender.sendMsq(mqName);
    });
}

void EasyMathServiceDeploy::responseScore(const int *score)
{
    mClientManager.deploy([this, score](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::EasyMath_RespGameOver);
        mMqSender.addParam(score);
        mMqSender.sendMsq(mqName);
    });
}

}

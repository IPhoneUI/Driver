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
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_EasyMath_RespServiceReady);
    mMqSender.sendMsq(clientName);
}

void EasyMathServiceDeploy::responseStartGame()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_EasyMath_RespStartGame);
        mMqSender.sendMsq(mqName);
    });
}

void EasyMathServiceDeploy::responseExpressionChanged(const ExpressionInfo &info)
{
    mClientManager.deploy([this, info](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_EasyMath_RespExpressionChange);
        mMqSender.addParam(info.firstNumber);
        mMqSender.addParam(info.secondNumber);
        mMqSender.addParam(static_cast<int>(info.exprType));
        mMqSender.addParam(info.dummyResult);
        mMqSender.sendMsq(mqName);
    });
}

void EasyMathServiceDeploy::responseTimeIntervalUpdated(size_t interval)
{
    mClientManager.deploy([this, interval](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_EasyMath_TimeIntervalUpdated);
        mMqSender.addParam(static_cast<int>(interval));
        mMqSender.sendMsq(mqName);
    });
}

void EasyMathServiceDeploy::responseGameOver(int score)
{
    mClientManager.deploy([this, score](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_EasyMath_RespGameOver);
        mMqSender.addParam(score);
        mMqSender.sendMsq(mqName);
    });
}

}

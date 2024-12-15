#include "EasyMathServiceImpl.h"
#include <utils/Logger.h>

namespace service {

EasyMathServiceImpl::EasyMathServiceImpl()
    : BaseServiceImpl(EasyMathServiceDeploy::instance())
    , mMonitors(new MonitorProgress(8))
{
    mDeploy = EasyMathServiceDeploy::instance();
    mEasyMathServer = driver::EasyMathServer::getInstance();
    mMonitors->setCommander(std::bind(&EasyMathServiceImpl::submitCommand, this, std::placeholders::_1));
}

void EasyMathServiceImpl::onMsqReceived()
{
    LOG_INFO("THAIVD --- EasyMathServiceImpl::onMsqReceived");
    std::vector<std::string> messages = mMqReceiver.receive("/EasyMathReq");
    if (!messages.empty()) {
        base::msq::Msq_EasyMathReq type = static_cast<base::msq::Msq_EasyMathReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::EasyMath_RegisterClient: {
            LOG_INFO("THAIVD --- EasyMathServiceImpl::onMsqReceived 111111111");
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::EasyMath_ReqStartGame: {
            mEasyMathServer->startGame();
            break;
        }
        case base::msq::EasyMath_ReqCheckResult: {
            LOG_INFO("THAIVD -- EasyMathServiceImpl::requestCheckingResult");
            bool result = mMqReceiver.get<bool>(messages[1]);
            mEasyMathServer->requestCheckingResult(result);
            break;
        }
        default:
            break;
        }
    }

}

void EasyMathServiceImpl::initialize()
{
    LOG_INFO("EasyMathServiceImpl initialize");
    Connection::connect(mEasyMathServer->onStartGame, std::bind(&EasyMathServiceImpl::onStartGame, this, std::placeholders::_1));
    Connection::connect(mEasyMathServer->onExpressionChanged, std::bind(&EasyMathServiceImpl::onExpressionChanged, this, std::placeholders::_1));
    Connection::connect(mEasyMathServer->onAnswerResult, std::bind(&EasyMathServiceImpl::onAnswerResult, this, std::placeholders::_1));
}

void EasyMathServiceImpl::finialize()
{
    LOG_INFO("EasyMathServiceImpl finialize");
}

void EasyMathServiceImpl::registerClient(const std::string &clientName)
{
    if (mDeploy->registerClient(clientName)) {
        mDeploy->responseServiceReady(clientName);
    }
}

void EasyMathServiceImpl::onStartGame(const bool &isStart)
{
    mDeploy->responseStartGame(isStart);
}

void EasyMathServiceImpl::onExpressionChanged(const ExpressionInfo &info)
{
    LOG_INFO("THAIVD --- EasyMathServiceImpl::onExpressionChanged: %d -- %d", info.firstNumber, info.secondNumber);
    mDeploy->responseExpressionChanged(info);
    mMonitors->start();
}

void EasyMathServiceImpl::onAnswerResult(const bool &result)
{
    LOG_INFO("THAIVD --- EasyMathServiceImpl::onAnswerResult %d", static_cast<int>(result));
    if (true == result) {
        mMonitors->setState(MonitorProgress::State::Correct);
    } else {
        mMonitors->setState(MonitorProgress::State::Incorrect);
    }
}

void EasyMathServiceImpl::submitCommand(int command)
{
    GameEvent event = static_cast<GameEvent>(command);

    switch (event)
    {
    case GameEvent::GameOver:
        mDeploy->responseScore(mEasyMathServer->getScore());
        mEasyMathServer->resetGame();
        break;
    case GameEvent::NextLevel:
        mEasyMathServer->nextLevel();
        break;
    default:
        break;
    }
}

}

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

void EasyMathServiceImpl::registerClient(const std::string &clientName)
{
    if (mDeploy->registerClient(clientName)) {
        mDeploy->responseServiceReady(clientName);
    }
}

void EasyMathServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/EasyMathReq");
    if (!messages.empty()) {
        base::msq::Msq_EasyMathReq type = static_cast<base::msq::Msq_EasyMathReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_EasyMath_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_EasyMath_ReqStartGame: {
            startGame();
            break;
        }
        case base::msq::Msq_EasyMath_ReqCheckResult: {
            bool result = mMqReceiver.get<bool>(messages[1]);
            requestCheckingResult(result);
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
    Connection::connect(mEasyMathServer->onHighestScoreUpdated, std::bind(&EasyMathServiceImpl::onHighestScoreChanged, this));
    Connection::connect(mEasyMathServer->onRangeNumberUpdated, std::bind(&EasyMathServiceImpl::onRangeNumberChanged, this));
}

void EasyMathServiceImpl::finialize()
{
    LOG_INFO("EasyMathServiceImpl finialize");
}

void EasyMathServiceImpl::startGame()
{
    if (mIsGameRunning == true)
        return;
    mIsGameRunning = true;
    mDeploy->responseStartGame(mIsGameRunning);
    generateExpression();
}

void EasyMathServiceImpl::generateExpression()
{
    std::vector<service::ExpressionType> operators = {service::ExpressionType::Addition, service::ExpressionType::Subtraction, service::ExpressionType::Multiplication};
    size_t m_nRandomOperator = floor(((float) rand() / (RAND_MAX)) * operators.size());
    service::ExpressionType expr = operators[m_nRandomOperator];

    int firstNumber = getRandomNumber(1 * mLevel, 5 * mLevel);
    int secondNumber = getRandomNumber(1 * mLevel, 5 * mLevel);
    int result = 0;

    switch (expr) {
    case service::ExpressionType::Addition:
        result = firstNumber + secondNumber;
        break;
    case service::ExpressionType::Subtraction:
        result = firstNumber - secondNumber;
        break;
    case service::ExpressionType::Multiplication:
        result = firstNumber * secondNumber;
        break;
    default:
        break;
    }

    mResult = result;

    mExprInfo.firstNumber = firstNumber;
    mExprInfo.secondNumber = secondNumber;
    mExprInfo.dummyResult = getResult();
    mExprInfo.exprType = expr;

    mDeploy->responseExpressionChanged(mExprInfo);
    mMonitors->start();
}

int EasyMathServiceImpl::getRandomNumber(const int &min, const int &max)
{
    return floor(((float) rand() / (RAND_MAX)) * (mRangeNum - 0) + 0);
}

void EasyMathServiceImpl::requestCheckingResult(const bool &result)
{
    if (mRandomResult == result) {
        mMonitors->setState(MonitorProgress::State::Correct);
    } else {
        mMonitors->setState(MonitorProgress::State::Incorrect);
    }
}

int EasyMathServiceImpl::getResult()
{
    mRandomResult = ((float) rand() / (RAND_MAX)) >= 0.5;
    if (mRandomResult == false) {
        mResult = getRandomNumber(mResult - 10, mResult + 10);
    }
    return mResult;
}

void EasyMathServiceImpl::resetGame()
{
    mIsGameRunning = false;
    mResult = 0;
    mFirstArgument = 0;
    mSecondArgument = 0;
    mRandomResult = 0;
    mLevel = 0;
    mRangeNum = 10;
    mScore = 0;
}

void EasyMathServiceImpl::nextLevel()
{
    mLevel++;
    mRangeNum += 5;
    mScore += 3;
    generateExpression();
}

void EasyMathServiceImpl::onHighestScoreChanged()
{
    mScore = mEasyMathServer->getHighestScore();
}

void EasyMathServiceImpl::onRangeNumberChanged()
{
    mRangeNum = mEasyMathServer->getRangeNumber();
}

void EasyMathServiceImpl::submitCommand(int command)
{
    GameEvent event = static_cast<GameEvent>(command);
    switch (event)
    {
    case GameEvent::GameOver:
        mDeploy->responseScore(mScore);
        break;
    case GameEvent::NextLevel:
        nextLevel();
        break;
    default:
        break;
    }
}

}

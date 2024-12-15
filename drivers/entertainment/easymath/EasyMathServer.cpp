#include "EasyMathServer.h"
#include <utils/Logger.h>
#include <cmath>

namespace driver {

static EasyMathServer* gInstance = 0;

EasyMathServer::EasyMathServer()
{
}

EasyMathServer* EasyMathServer::getInstance()
{
    if (gInstance == nullptr)
    {
        // throw std::runtime_error("The EasyMathServer has not been initialized yet");
        gInstance = new EasyMathServer();
    }

    return gInstance;
}

void EasyMathServer::initialize()
{
    LOG_INFO("EasyMathServer initialize");
    if (gInstance == nullptr)
    {
        gInstance = new EasyMathServer();
    }
}

void EasyMathServer::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void EasyMathServer::startGame()
{
    if (mIsGameRunning == true)
        return;
    mIsGameRunning = true;
    onStartGame.emit(mIsGameRunning);
    generateExpression();
}

void EasyMathServer::generateExpression()
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

    onExpressionChanged.emit(mExprInfo);

}

int EasyMathServer::getRandomNumber(const int &min, const int &max)
{
    return floor(((float) rand() / (RAND_MAX)) * (mRangeNum - 0) + 0);
}

void EasyMathServer::requestCheckingResult(const bool &result)
{
    if (result == mRandomResult) {
        onAnswerResult.emit(true);
    } else {
        onAnswerResult.emit(false);
    }
}

int EasyMathServer::getResult()
{
    mRandomResult = ((float) rand() / (RAND_MAX)) >= 0.5;
    if (mRandomResult == false) {
        mResult = getRandomNumber(mResult - 10, mResult + 10);
    }
    return mResult;
}

void EasyMathServer::resetGame()
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

void EasyMathServer::nextLevel()
{
    mLevel++;
    mRangeNum += 5;
    generateExpression();
}

int EasyMathServer::getScore() const
{
    return mScore;
}

}

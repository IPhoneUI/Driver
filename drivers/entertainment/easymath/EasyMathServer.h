#ifndef EASYMATHSERVER_H
#define EASYMATHSERVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <easymath/EasyMathDef.h>

namespace driver {

class EasyMathServer : public common::BaseDriver 
{
public:
    static EasyMathServer* getInstance();
    static void initialize();
    void connectDriver() override;

    void startGame();
    void generateExpression();
    int getRandomNumber(const int& min, const int& max);
    void requestCheckingResult(const bool& result);
    int getResult();
    void resetGame();
    void nextLevel();

    int getScore() const;

    Signal<bool> onStartGame;
    Signal<const service::ExpressionInfo&> onExpressionChanged;
    Signal<int> onGameOver;
    Signal<bool> onAnswerResult;

private:
    explicit EasyMathServer();

    service::ExpressionInfo mExprInfo;

    bool mIsGameRunning {false};
    int mResult {0};
    bool mRandomResult {false};
    int mFirstArgument {0};
    int mSecondArgument {0};
    int mResultDummy {0};
    int mLevel {0};
    int mRangeNum {10};
    std::mutex mMutex;
    int mScore {0};
};

}

#endif //EASYMATHSERVER_H

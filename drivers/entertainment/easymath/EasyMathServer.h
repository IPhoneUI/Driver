#ifndef EASYMATHSERVER_H
#define EASYMATHSERVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <easymath/EasyMathDef.h>
#include "MonitorProgress.h"

namespace driver {

class EasyMathServer : public common::BaseDriver 
{
public:
    static EasyMathServer* getInstance();
    static void initialize();

    void connectDriver() override;
    void writeBuffer() override;
    void execute(milliseconds delta) override;

    void setRangeNumber(const int& range);
    int getRangeNumber() const;

    void setHighestScore(const int& score);
    int getHighestScore() const;

    void startGame();
    void generateExpression();
    int getRandomNumber(const int& min, const int& max);
    void requestCheckingResult(const bool& result);
    int getResult();
    void resetGame();
    void nextLevel();
    void submitCommand(int command);

    Signal<> onHighestScoreUpdated;
    Signal<> onRangeNumberUpdated;
    Signal<> onStartGame;
    Signal<int> onGameOver;
    Signal<size_t> onTimeIntervalChanged;
    Signal<const service::ExpressionInfo&> onExpressionChanged;

private:
    explicit EasyMathServer();

    int mHighestCore {0};
    int mRangeNumber {0};
    std::mutex mMutex;

    MonitorProgress* mMonitors;
    service::ExpressionInfo mExprInfo;
    bool mIsGameRunning {false};
    int mResult {0};
    bool mRandomResult {false};
    int mFirstArgument {0};
    int mSecondArgument {0};
    int mResultDummy {0};
    int mLevel {0};
    int mScore {0};
    int mRangeNum {10};
};

}

#endif //EASYMATHSERVER_H

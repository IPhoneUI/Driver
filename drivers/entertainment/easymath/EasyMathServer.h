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
    void writeBuffer() override;

    void setRangeNumber(const int& range);
    int getRangeNumber() const;

    void setHighestScore(const int& score);
    int getHighestScore() const;

    Signal<> onHighestScoreUpdated;
    Signal<> onRangeNumberUpdated;

private:
    explicit EasyMathServer();

    int mHighestCore {0};
    int mRangeNumber {0};
};

}

#endif //EASYMATHSERVER_H

#ifndef EASYMATHSERVER_H
#define EASYMATHSERVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>

namespace driver {

class EasyMathServer : public common::BaseDriver 
{
public:
    static EasyMathServer* getInstance();
    static void initialize();
    void connectDriver() override;

    void startGame();

    Signal<bool> onStartGame;

private:
    explicit EasyMathServer();

    bool mIsStartGame {false};
};

}

#endif //EASYMATHSERVER_H

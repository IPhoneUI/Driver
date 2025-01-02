#ifndef SIMULATESERVER_H
#define SIMULATESERVER_H

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <future>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <algorithm>

namespace simulate {

class SimulateServer {
public:
    ~SimulateServer();
    static SimulateServer& instance();

    void start();
    void loop();
    
private:
    SimulateServer();

    bool mIsStarted = false;
    int  mSockfd = -1;
    struct pollfd mFds[1];

    std::thread mThread;
    bool mThreadRunning = false;
};

}

#endif // SIMULATESERVER_H
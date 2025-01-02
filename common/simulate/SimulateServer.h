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
#include <shared_mutex>
#include <mutex>
#include <map>

namespace simulate {

class SimulateServer {
public:
    ~SimulateServer();
    static SimulateServer& instance();

    struct Client 
    {
        Client(int fd, SimulateServer& parent) : 
            mSockfd(fd),
            mServer{parent}
        {
        }

        int  mSockfd = -1;
        struct pollfd mFds[1];
        std::thread mThread;
        bool mThreadRunning = false;
        bool mIsStarted = false;
        std::mutex mMuxMessages;
        SimulateServer& mServer;

        void start();
        void stop();
        void readData();
    };

    void start();
    void acceptClient();
    
private:
    SimulateServer();

    bool mIsStarted = false;
    int  mSockfd = -1;

    /* The struct pollfd is used for the poll() system call, which monitors multiple file descriptors for events (read, write, or error conditions).
        Syntax: 
        struct pollfd {
            int fd;        File descriptor to monitor (it could represent a socket, pipe, or regular file.)
            short events;  Events to monitor for (e.g., POLLIN, POLLOUT)
            short revents;  Events that occurred (set by poll()), Contains the events that actually occurred after calling poll().
        };
     * mFds is an array of size 1. Monitoring a single file descriptor for specific events.
     * Handling I/O on sockets or pipes in event-driven programming.
     * Typically, we fill the fields of mFds[0] before calling the poll() function.
     */
    struct pollfd mFds[1];

    std::thread mThread;
    bool mThreadRunning = false;
    std::map<int, Client*> mClients;
    std::shared_mutex mMuxClient;
};

}

#endif // SIMULATESERVER_H
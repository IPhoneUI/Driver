#include "SimulateServer.h"
#include <utils/Logger.h>
#include <BaseDriver.h>

namespace common {

SimulateServer::SimulateServer()
{
}

SimulateServer::~SimulateServer()
{
}

SimulateServer& SimulateServer::instance()
{
    static SimulateServer ins;
    return ins;
}

void SimulateServer::start()
{
    if(mIsStarted)
        return;

    int portno = 8080;
    struct sockaddr_in sock_addr;

    /* Create a socket
     * Syntax: socket(int domain, int type, int protocol)
     *** - AF_INET: IPv4
     *** - SOCK_STREAM: Use TCP protocol.
     *** - IPPROTO_TCP: TCP as the transport protocol.
     */ 
    mSockfd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (mSockfd < 0) 
    {
        LOG_ERR("Create a new socket fail!");
        return;
    }

    int opt = 1;

    /* Config socket options
     * Syntax: int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
     *** - sockfd: The file descriptor for the socket created by socket().
     *** - level: The protocol level for the option. Here, SOL_SOCKET refers to options at the socket API level.
     *** - optname: The name of the option being set, such as:
     ***      + SO_REUSEADDR: Allows reusing the IP address and port even if a socket using the same address/port combination is still in a "TIME_WAIT" state.
     ***      + SO_REUSEPORT: allows multiple processes to bind to the same port on the same IP address.
     *** - optval: A pointer to the value for the option.
     *** - optlen: The size of the value being set (optval).
     */
    if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        LOG_ERR("setsockopt fail");
        close(mSockfd);
        return;
    }
    if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        LOG_ERR("setsockopt fail");
        close(mSockfd);
        return;
    }

    /* This clears the memory occupied by sock_addr by setting all its bytes to 0. 
     * This ensures that the structure is completely initialized and does not contain garbage values from previous memory allocations.
     * If the structure contains uninitialized data, it may cause unpredictable behavior when used in network operations
     */
    bzero((char *) &sock_addr, sizeof(sock_addr));

    /* setup the host_addr structure for use in bind call 
     * Specifies the type of addresses that the socket can communicate with.
     * AF_INET means the socket will use IPv4 addresses.
     */
    sock_addr.sin_family = AF_INET;  

    /* Assign IP Address
     * sin_addr.s_addr: Holds the IP address of the server.
     *** - INADDR_ANY: Tells the server to listen on all available network interfaces on the machine.
     ***               If the machine has multiple network interfaces (e.g., Ethernet, Wi-Fi), INADDR_ANY binds the socket to all of them, 
                       allowing it to accept connections on any interface.
     */
    sock_addr.sin_addr.s_addr = INADDR_ANY;  

    /* Set Port
     * sin_port: Specifies the port number on which the server will listen for incoming connections.
     * The htons (Host TO Network Short) function converts the port number from host byte order (little-endian) to network byte order (big-endian).
     * Network protocols typically use big-endian format, so this conversion ensures the port number is transmitted correctly.
     * If the port number is not converted, the server might bind to an incorrect port, causing unexpected behavior.
     */
    sock_addr.sin_port = htons(portno);

    /* bind Function
     * Syntax: bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
     * bind() passes file descriptor, the address structure, and the length of the address structure
     *  - This bind() call will bind the socket to the current IP address on port, portno
     *  - The socket is tied to the specified address and port.
     *  - If another process is already using the same address and port, bind will fail, returning -1.
     * This ensures that the socket can be identified on the network and receive incoming connections on the specified address and port.
     */
    if (bind(mSockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        LOG_ERR("ERROR on binding\n");
        close(mSockfd);
        return;
    }

    /* listen Function
     * Syntax: int listen(int sockfd, int backlog);
     * This listen() call tells the socket to listen to the incoming connections.
     *  - The listen function puts the socket in a passive mode, meaning it starts waiting for incoming connection requests from clients.
     *  - The server socket is now ready to accept connections.
     *  - backlog: The maximum number of pending connection requests that can be queued if the server is busy processing other connections.
     * The listen() function places all incoming connection into a backlog queue until the server accepts them using accept().
     * If the backlog queue is full (more than 15 connections in this case), additional connection requests are bes ignored or received an error.
     */
    // 
    // The listen() function hold all incoming connection requests from clients into a backlog queue .
    // until accept() call accepts the connection.
    // Here, we set the maximum size for the backlog queue to 5.
    listen(mSockfd,15);

    mThread = std::thread( std::bind(&SimulateServer::acceptClient, this) );
    mThread.detach();

    mIsStarted = true;
}

void SimulateServer::acceptClient()
{
    mThreadRunning = true;

    /* the poll() system call to monitor this specific socket for events */
    mFds[0].fd = mSockfd;

    /* events: Specifies the types of events you want poll() to monitor for this socket. 
     * Indicates that there is data to read on the socket.
     * For example, when a client sends data to the socket.
     * When pass the mFds array to poll(), the system will:
     * Monitor the socket mSockfd for the specified events.
     * If either POLLIN or POLLPRI occurs on the socket, poll() will return, and you can check which event(s) occurred using the revents field.
     */
    mFds[0].events = POLLIN | POLLPRI;

    while(mThreadRunning) 
    {
        static struct sockaddr_in clientAddr;
        static socklen_t clientStructLen = sizeof(clientAddr);
        int newsockfd;

        /* Syntax: poll(mFds, 1, 10)
         * - mFds: Array of struct pollfd that monitors file descriptors for events.
           - 1: Number of file descriptors to monitor (in this case, just mSockfd).
           - 10: Timeout in milliseconds (10 ms). This means poll() will wait for 10 ms (block 10ms thread) to see if an event occurs.
         * If poll() returns a non-zero value, it means one or more events occurred on the monitored file descriptor
         * If poll() returns 0, it indicates a timeout (no events occurred in 10 ms).
         * If poll() returns -1, it indicates an error.
         */

        int rv = poll(mFds, 1, 10);
        if (rv > 0) 
        {
            /* Accepts the new connection on the server socket mSockfd.
             * Creates a new socket newsockfd for the specific client.
             * Fills clientAddr with the client's address information.
             * Returns newsockfd, a file descriptor for the new connection, or -1 on error.
             */
            newsockfd = accept(mSockfd, (struct sockaddr *) &clientAddr, &clientStructLen);

            if (newsockfd < 0)
            { 
                continue;
            }

            LOG_INFO("Got connection from %s port %d",
                        inet_ntoa(clientAddr.sin_addr), 
                        ntohs(clientAddr.sin_port));
            
            Client* client = new Client(newsockfd, *this);

            mMuxClient.lock();
            mClients[newsockfd] = client;
            mMuxClient.unlock();
            client->start();
        }
        else 
        {
            if (mMsqQueue.size() > 0)
            {
                Message front = mMsqQueue.front();
                mMsqQueue.pop();

                for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
                {
                    (*it).second->onSimulateReceived(front.topic, front.option, front.content);
                }
            }
        }
    }

    if(mSockfd > -1)
    {
        close(mSockfd);
    }
}

void SimulateServer::registerDriver(const std::string& name, BaseDriver* driver)
{
    for (auto it = mDrivers.begin(); it != mDrivers.end(); ++it)
    {
        if ((*it).first == name)
        {
            LOG_WARN("%s is found! reject to register this driver", name.c_str());
            return;
        }
    }

    mDrivers.emplace(name, driver);
}

void SimulateServer::Client::start()
{
    if(mIsStarted)
        return;

    mThread = std::thread( std::bind(&SimulateServer::Client::readData, this) );
    mThread.detach();

    mIsStarted = true;
}

void SimulateServer::Client::stop()
{
    mThreadRunning = false;
}

void SimulateServer::Client::readData()
{
    mThreadRunning = true;
    mFds[0].fd = mSockfd;
    mFds[0].events = POLLIN | POLLPRI;
    static char buff[5000];
    while(mThreadRunning)
    {
        int rv = poll(mFds, 1, 10);

        if (rv > 0) 
        {
            /* Types of events that actually occurred.
             * POLLIN indicates that there is data coming from the mSockfd socket (data is ready to read).
             * poll() ensures that the socket has data ready to read (POLLIN) before calling read.
             * So in this case, read will not block thread, because it is only called when poll() confirms that there is data.
             */
            if (mFds[0].revents & POLLIN)
            {
                /* Read data from socket mSockfd into buffer buff.
                 * Return number of bytes read (n). 
                 */
                int n = read(mSockfd,buff,5000);
                //
                if (n > 0 && n < 5000)
                {
                    std::string str;
                    str.resize(n);
                    memcpy(str.data(), buff, n);

                    std::vector<std::string> data = utils::StringUtils::split(str, "#");

                    Message message;
                    message.topic = data[0];
                    message.option = data[1];
                    message.content = data[2];

                    mServer.mMsqQueue.push(message);
                }
            }
        }
    }
    if(mSockfd != -1)
    {
        close(mSockfd);
    }
}

}
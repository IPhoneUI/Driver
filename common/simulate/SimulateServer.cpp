#include "SimulateServer.h"
#include <utils/Logger.h>
#include <functional>

namespace simulate {

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

    mThread = std::thread( std::bind(&SimulateServer::loop, this) );
    mThread.detach();

    mIsStarted = true;
}

void SimulateServer::loop()
{
    mThreadRunning = true;

    mFds[0].fd = mSockfd;
    mFds[0].events = POLLIN | POLLPRI;

    while(mThreadRunning) 
    {
        static struct sockaddr_in clientAddr;
        static socklen_t clientStructLen = sizeof(clientAddr);
        int newsockfd;
        if (poll(mFds, 1, 10)) 
        {
            newsockfd = accept(mSockfd, (struct sockaddr *) &clientAddr, &clientStructLen);

            if (newsockfd < 0)
            { 
                continue;
            }

            LOG_INFO("New connection accepted.");
        }
        else 
        {
            char buffer[1024];
            bzero(buffer, sizeof(buffer));
            ssize_t bytesRead = read(newsockfd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) 
            {
                buffer[bytesRead] = '\0';
                LOG_INFO("Received message: %s", buffer);
            }
        }
        
    }

    if(mSockfd > -1)
    {
        close(mSockfd);
    }

}


}

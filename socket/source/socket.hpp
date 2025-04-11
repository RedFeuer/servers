#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <vector>
#include <string>
#include <cstddef>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace network {
    class Socket {
    public:
        Socket();
        ~Socket();

        bool create();
        bool bind(int port);
        bool lister(int backlog = 5);
        bool accept(Socket& clientSocket);
        bool connect(const std::string& host, int port);
        int send(const std::string& data);
        int send(const std::vector<char>& data);
        int receive(std::string& buffer);
        int receive(std::vector<char>& buffer);
        void close();

    private:
        #ifdef _WIN32
        SOCKET sockfd = INVALID_SOCKED;
        #else
        int sockfd = -1;
        #endif
        bool isClosed = true;
    };
}

#include "socket.hpp"
#include <stdexcept>
#include <cstring>

namespace network {
    /*иницилизация Winsock*/
    #ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
    class WSAInitializer {
    public:
        WSAInitializer() {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
                throw std::runtime_error("WSAStartup failed");
            }
        }
        ~WSAInitializer() {WSACleanup();};
    };
    static WSAInitializer wsaInitializer;
    #endif

    Socket::Socket() {
        create(); // создаем сокет
    }

    Socket::~Socket() {
        close(); // закрываем сокет
    }

    bool Socket::create() {
        /*0 - для TCP*/
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        #ifdef _WIN32
        if (sockfd == INVALID_SOCKET) {return false;}
        #else
        if (sockfd < 0) {return false;}
        #endif
        isClosed = false; // открыт
        return true;
    }

    bool Socket::bind(int port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;  // IPv4
        addr.sin_addr.s_addr = INADDR_ANY; // Все интерфейсы(0.0.0.0)
        addr.sin_port = htons(port); // Порт в сетевом порядке байт(конвертим)

        #ifdef _WIN32
        return ::bind(sockfd, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
        #else
        return ::bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == 0;
        #endif
    }

    bool Socket::listen(int backlog) {
        #ifdef _WIN32
        return ::listen(sockfd, backlog) != SOCKET_ERROR;
        #else
        return ::listen(sockfd, backlog) == 0;
        #endif
    }

    bool Socket::accept(Socket &clientSocket) {
        sockaddr_in clientAddr{};    // хранит Host и Port клиента
        socklen_t len = sizeof(clientAddr);
        clientSocket.sockfd = ::accept(sockfd, (sockaddr*)&clientAddr, &len); // создание клиентского сокета
        #ifdef _WIN32
        return clientSocket.sockfd != INVALID_SOCKET;
        #else
        return clientSocket.sockfd >= 0;
        #endif
    }

    bool Socket::connect(const std::string &host, int port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        #ifdef _WIN32
        if (InetPton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
            return false;
        }
        #else
        if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) { // IPv4 -> бинарный формат
            return false;
        }
        #endif
        return ::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == 0;
    }

    int Socket::send(const std::string &data) {
        return ::send(sockfd, data.data(), data.size(), 0);
    }

    int Socket::receive(std::string &buffer) {
        return ::recv(sockfd, buffer.data(), buffer.size(), 0);
    }

    void Socket::close() {
        if (!isClosed) {
            #ifdef _WIN32
            closesocket(sockfd)
            #else
            ::close(sockfd);
            #endif
            isClosed = true;
        }
    }


}
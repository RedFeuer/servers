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


}
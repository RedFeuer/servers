#include "socket.hpp"
#include <stdexcept>
#include <cstring>

#include <iostream>

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
        int opt = 1;
        #ifdef _WIN32
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
        #else
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        #endif


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

    ssize_t Socket::send(const std::string &data) {
        return ::send(sockfd, data.data(), data.size(), 0);
    }

    /*кривая буферная реализация, мб потом перепишу поумнее, но пока работает*/
    ssize_t Socket::receive(std::string &buffer) {
        char tempBuffer[4096];
        ssize_t totalBytes = 0;

        while (true) {
            ssize_t bytesRead = ::recv(sockfd, tempBuffer, sizeof(tempBuffer), 0);
            if (bytesRead <= 0) break; // ошибка или конец данных

            buffer.append(tempBuffer, bytesRead);
            totalBytes += bytesRead;

            /* проверяем на завершение HTTP-запроса (наличие \r\n\r\n) */
            if (buffer.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        }

        return totalBytes;
    }

    void Socket::close() {
        if (!isClosed) {
        #ifdef _WIN32
//            closesocket(sockfd)
            if (sockfd != INVALID_SOCKET) {
                closesocket(sockfd);
            }
        #else
            if (sockfd != -1) {
                ::close(sockfd);
            }
//            ::close(sockfd);
        #endif
            isClosed = true;
        }
    }


}
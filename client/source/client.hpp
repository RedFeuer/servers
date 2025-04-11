#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"
#include <string>

namespace network {
    class Client {
    public:
        Client(const std::string& serverHost, int serverPort);  // для коннекта к data_server
        ~Client();

        bool connectToServer();
        void sendData(const std::string& data);
        bool receiveAck();

    private:
        Socket clientSocket;
        std::string serverHost;
        int serverPort;
    };
}

#endif
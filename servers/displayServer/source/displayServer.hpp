#ifndef DISPLAYSERVER_HPP
#define DISPLAYSERVER_HPP

#include "server.hpp"

namespace network {
    class DisplayServer : public Server {
    public:
        ~DisplayServer();

        void start(int port) override;
        void stop() override;
        void handleClient(Socket& client) override;

    private:
        Socket serverSocket;  // для приема данных от dataServer
        bool isRunning = false;
    };
}

#endif
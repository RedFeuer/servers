#ifndef DISPLAYSERVER_HPP
#define DISPLAYSERVER_HPP

#include "server.hpp"

namespace network {
    class DisplayServer : public Server {
    public:
        ~DisplayServer() override;

        void start(int port) override;
        void stop() override;
        void handleClient(Socket& client) override;

        [[nodiscard]] Socket & getServerSocket() {return serverSocket;}

    private:
        Socket serverSocket;  // для приема данных от dataServer
        bool isRunning = false;
    };
}

#endif
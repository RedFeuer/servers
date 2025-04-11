#include "Socket.hpp"

namespace network {
    /*абстрактный класс*/
    class Server {
    public:
        virtual ~Server() = default;
        virtual void start(int port) = 0;
        virtual void stop() = 0;
        virtual void handleClient(Socket& client) = 0;
    };
}
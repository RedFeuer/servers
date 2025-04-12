#ifndef DATASERVER_HPP
#define DATASERVER_HPP

#include "server.hpp"
#include <string>

namespace network {
    class DataServer : public Server {
    public:
        DataServer(const std::string& displayHost, int displayPort); // сообщаем координаты сервера отображения
        ~DataServer() override;

        void start(int port) override;
        void stop() override;
        void handleClient(Socket& client) override;

    private:
        Socket serverSocket;   // для приема клиентов
        Socket displaySocket; // для отправки данных на сервер отображения

        std::string displayHost;
        int displayPort;
        bool isRunning = false;

        std::string processData(const std::string& input); // функция обработки введенной строки
    };
}

#endif
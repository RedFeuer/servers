#include "dataServer.hpp"
#include <iostream>
#include <csignal>

//using namespace network;

network::DataServer* server = nullptr;

//void signalHandler(int signum) {
//    if (server) {
//        std::cout << "\nОстановка сервера..." << std::endl;
//        server->stop();
//    }
//    std::_Exit(0);
//}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: data_server_exec <data_port> <displayport>\n";
    }

    std::string displayHost = "localhost"; // IP сервера отображения
    int displayPort = std::stoi(argv[1]);               // порт сервера отображения
    int dataPort = std::stoi(argv[2]);                // порт текущего data-сервера

    // Назначаем обработчик сигнала прерывания (Ctrl+C)
//    std::signal(SIGINT, signalHandler);

    try {
        server = new network::DataServer(displayHost, displayPort);
        server->start(dataPort);
    } catch (const std::exception& e) {
        std::cerr << "Error starting data server: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#include "displayServer.hpp"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<network::DisplayServer> server;

//void signalHandler(int signal) {
//    if (server) {
//        std::cout << "\nStopping display server...\n";
//        server->stop();
//    }
//    std::_Exit(0);
//}

int main(int argc, char* argv[]) {
    /*инструкция по применению(показывает, чтобы ты порт написал в консоли)*/
    if (argc < 2) {
        std::cerr << "Usage: display_server_exec <port>\n";
        return 1;
    }

    /*получаем port*/
    int port = std::stoi(argv[1]);

//    std::cout << port << std::endl << std::endl;

//    std::signal(SIGINT, signalHandler);

    server = std::make_unique<network::DisplayServer>();

    try {
        server->start(port);
    } catch (const std::exception& ex) {
        std::cerr << "Error starting DisplayServer: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

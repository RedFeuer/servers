#include "dataServer.hpp"
#include <iostream>
#include <csignal>
#include <memory>

//using namespace network;

std::unique_ptr<network::DataServer> server;


/*
 * не вызываем std::_Exit(0), потому что он не вызывает деструкторы!
 * вместо этого просто стопим сервер и возвращаем управление main*/
void signalHandler([[maybe_unused]] int signal) {
    if (server) {
        std::cout << "\nStopping DATA server..." << std::endl;
        server->stop();
    }
    server.reset(); // удаляем указатель и вызываем деструктор
    std::_Exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: data_server_exec <data_port> <displayport>\n";
    }

    std::string displayHost = "127.0.0.1"; // IP сервера отображения
    int displayPort = std::stoi(argv[1]);               // порт сервера отображения
    int dataPort = std::stoi(argv[2]);                // порт текущего data-сервера

    /* при нажатии Ctrl+C происходит не завершение прочесса через std::_Exit(0), а вызов функции signalHandler,
    * которая стопит сервер по-нормальному*/
    std::signal(SIGINT, signalHandler);

    try {
        server = std::make_unique<network::DataServer>(displayHost, displayPort);
        server->start(dataPort);
    } catch (const std::exception& e) {
        std::cerr << "Error starting data server: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

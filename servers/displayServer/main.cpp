#include "displayServer.hpp"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<network::DisplayServer> server;


/*
 * не вызываем std::_Exit(0), потому что он не вызывает деструкторы!
 * вместо этого просто стопим сервер и возвращаем управление main*/
void signalHandler(int signal) {
    if (server) {
        std::cout << "\nStopping DISPLAY server...\n";
        server->stop();  /* прерываем цикл в main */
    }
}

int main(int argc, char* argv[]) {
    /*инструкция по применению(показывает, чтобы ты порт написал в консоли)*/
    if (argc < 2) {
        std::cerr << "Usage: display_server_exec <port>\n";
        return 1;
    }

    /*получаем port*/
    int port = std::stoi(argv[1]);

//    std::cout << port << std::endl << std::endl;

    /* при нажатии Ctrl+C происходит не завершение прочесса через std::_Exit(0)(когда деструктор не вызывается,
     * что ломает поведение программы и вызывает утечки в still_reachable), а вызов функции signalHandler,
     * которая стопит сервер по-нормальному*/
    std::signal(SIGINT, signalHandler);

    server = std::make_unique<network::DisplayServer>();

    try {
        server->start(port);
    } catch (const std::exception& ex) {
        std::cerr << "Error starting DisplayServer: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

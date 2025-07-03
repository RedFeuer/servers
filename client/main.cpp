// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

#include "client.hpp"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<network::Client> client;

//void signalHandler(int signal) {
//    std::cout << "\nClient exiting...\n";
//    std::_Exit(0);
//}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: client_exec <server_host> <server_port>\n";
        return 1;
    }

    std::string host = argv[1];
    int port = std::stoi(argv[2]);
    // std::string message = argv[3]; // так было раньше, когда формат общения был client_exec <server_host> <server_port> <message>
//    std::cout << host << std::endl << port << std::endl << message << std::endl; // отладочка

//    std::signal(SIGINT, signalHandler);

//    client = std::make_unique<network::Client>(host, port);
//
//    if (!client->connectToServer()) {
//        std::cerr << "Failed to connect to server " << host << ":" << port << "\n";
//        return 1;
//    }

    /*ТУТ ДЕЛАТЬ ЧТЕНИЕ СООБЩЕНИЙ В ЦИКЛЕ И ПОСЛЕДОВАТЕЛЬНУЮ ОТПРАВКУ НА СЕРВЕР*/
    std::string message;
    while (true) {
        /*каждый раз создаем новый коннект с data_server, иначе это не новое соединение
         * и if (serverSocket.accept(clientSocket)) не пропустит к обработке данных
         * и отправке на сервер*/
        client = std::make_unique<network::Client>(host, port);
        if (!client->connectToServer()) {
            std::cerr << "Failed to connect to server " << host << ":" << port << "\n";
            return 1;
        }

        std::cout << "Enter message (or 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") break;

        client->sendData(message);

        if (client->receiveAck()) {
            std::cout << "Server acknowledged the message\n";
        } else {
            std::cout << "No acknowledgment from data server\n";
        }
    }

    return 0;
}

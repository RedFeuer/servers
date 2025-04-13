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
    if (argc < 4) {
        std::cerr << "Usage: client_exec <server_host> <server_port> <message>\n";
        return 1;
    }

    std::string host = argv[1];
    int port = std::stoi(argv[2]);
    std::string message = argv[3];
//    std::cout << host << std::endl << port << std::endl << message << std::endl;

//    std::signal(SIGINT, signalHandler);

    client = std::make_unique<network::Client>(host, port);

    if (!client->connectToServer()) {
        std::cerr << "Failed to connect to server " << host << ":" << port << "\n";
        return 1;
    }

    /*ТУТ ДЕЛАТЬ ЧТЕНИЕ СООБЩЕНИЙ В ЦИКЛЕ И ПОСЛЕДОВАТЕЛЬНУЮ ОТПРАВКУ НА СЕРВЕР*/
    client->sendData(message);

    if (client->receiveAck()) {
        std::cout << "Server acknowledged the message.\n";
    } else {
        std::cout << "No acknowledgment from server.\n";
    }

    return 0;
}

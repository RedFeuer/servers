// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

#include "client.hpp"
#include <iostream>

namespace network {

    Client::Client(const std::string& serverHost, int serverPort)
    : serverHost(serverHost), serverPort(serverPort) {};

    Client::~Client() {clientSocket.close();};

    bool Client::connectToServer() {
        return clientSocket.connect(serverHost, serverPort);
    }

    void Client::sendData(const std::string &data) {
        nlohmann::json requestJson;
        requestJson["data"] = data;

        std::string request = "POST /process HTTP/1.1\r\n"         // метод запроса и путь
                              "Host: " + serverHost + "\r\n"       // адрес сервера
                              "Content-Type: application/json\r\n" // тип содержимого
                              "Content-Length: " + std::to_string(requestJson.dump().size()) + "\r\n" // длина тела запроса
                              "\r\n" + requestJson.dump();   // тело запроса

        clientSocket.send(request);

//        std::cout << data << std::endl << std::endl << request << std::endl << std::endl;
    }

    bool Client::receiveAck() {
        std::string response;
        clientSocket.receive(response);
        std::cout << response << std::endl << std::endl;
        return response.find("HTTP/1.1 200 OK") != std::string::npos;
    }
}
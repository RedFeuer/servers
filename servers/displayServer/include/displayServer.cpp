#include "displayServer.hpp"
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

namespace network {
    DisplayServer::~DisplayServer() {stop();}

    void DisplayServer::start(int port) {
        if (!serverSocket.bind(port)) {
            throw std::runtime_error("Bind failed");
        }
        if (!serverSocket.listen()) {
            throw std::runtime_error("Listen failed");
        }
        isRunning = true;

        while (isRunning) {
            Socket dataServerSocket;
            if (serverSocket.accept(dataServerSocket)) {
                handleClient(dataServerSocket);
            }
            else {
//                dataServerSocket.close();
                throw std::runtime_error("Connection error(Data_Server(as client)->Display_Server)");
            }
//            dataServerSocket.close();
        }
    }

    void DisplayServer::stop() {
        if (isRunning) {
            serverSocket.close();
            isRunning = false;
        }
    }

    void DisplayServer::handleClient(network::Socket &client) {
//        std::ofstream log("display_server.log", std::ios::app); // для логов для тестов

        std::string request;
        client.receive(request);

//        std::cout<< request << std::endl << std::endl;

        std::size_t body_pos = request.find("\r\n\r\n");
        if (body_pos == std::string::npos) {
            throw std::invalid_argument("Invalid HTTP request");
        }

        std::string jsonBody = request.substr(body_pos + 4);
        nlohmann::json inputJson = nlohmann::json::parse(jsonBody);

        std::cout << "Received data: " << inputJson["result"] << std::endl;

        /*сообщаем data_server, что инфа пришла*/
        std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 0\r\n"
                "\r\n";
        client.send(response);

//        log << "Receiver: " << inputJson["result"] << std::endl;
    }
}
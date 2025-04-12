#include "dataServer.hpp"
#include <nlohmann/json.hpp>

namespace network {
    DataServer::DataServer(const std::string &displayHost, int displayPort)
    : displayHost(displayHost), displayPort(displayPort) {}

    DataServer::~DataServer() {
//        serverSocket.close();
//        displaySocket.close();
        stop();
    };

    void DataServer::start(int port) {
        /*не смогли привязать сокет к порту*/
        if (!serverSocket.bind(port)) {throw std::runtime_error("Bind to client port" + std::to_string(port) + "failed");}
        /*не смогли активировать прослушивание*/
        if (!serverSocket.listen()) {throw std::runtime_error("Listen failed");}
        isRunning = true;
    }

    void DataServer::stop() {
        if (isRunning) {
            serverSocket.close();
            displaySocket.close();
            isRunning = false;
        }
    }

    void DataServer::handleClient(network::Socket &client) {

    }
}
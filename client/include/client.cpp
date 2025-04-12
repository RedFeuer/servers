#include "client.hpp"

namespace network {

    Client::Client(const std::string& serverHost, int serverPort)
    : serverHost(serverHost), serverPort(serverPort) {};

    Client::~Client() {clientSocket.close();};

    bool Client::connectToServer() {
        return clientSocket.connect(serverHost, serverPort);
    }
}
#include <gtest/gtest.h>
#include "client.hpp"
#include "dataServer.hpp"
#include "displayServer.hpp"
#include "test_servers.hpp"

TEST(Client, Connect) {
    std::string dataHost = "127.0.0.1";
    int dataPort =  9080;

    network::Client client(dataHost, dataPort);

    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);
    dataServer.getServerSocket().bind(dataPort);
    dataServer.getServerSocket().listen();

    ASSERT_EQ(client.connectToServer(), true);
}

TEST(Client, sendData) {
    std::string dataHost = "127.0.0.1";
    int dataPort =  9080;

    network::Client client(dataHost, dataPort);

    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);
    dataServer.getServerSocket().bind(dataPort);
    dataServer.getServerSocket().listen();

    client.connectToServer();

    std::string data = "HELLO WORLD";
    ASSERT_NO_FATAL_FAILURE(client.sendData(data));

//    network::DisplayServer displayServer;
//    displayServer.getServerSocket().bind(displayPort);
//    displayServer.getServerSocket().listen();
//
//    dataServer.handleClient(client.getClientSocket());
//
//    /*отправили данные и они дошли*/
//    ASSERT_EQ(client.receiveAck(), true);
}
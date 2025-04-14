#include <gtest/gtest.h>
#include "client.hpp"
#include "dataServer.hpp"
#include "displayServer.hpp"

TEST(DataServer, stop) {
    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);
    dataServer.setIsRunning(true);
    ASSERT_EQ(dataServer.getIsRunning(), true);

    ASSERT_NO_FATAL_FAILURE(dataServer.stop());
    ASSERT_EQ(dataServer.getIsRunning(), false);
}
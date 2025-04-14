#include <gtest/gtest.h>
#include "client.hpp"
#include "dataServer.hpp"
#include "displayServer.hpp"

TEST(DisplayServer, stop) {
    network::DisplayServer displayServer;
    displayServer.setIsRunning(true);
    ASSERT_EQ(displayServer.getIsRunning(), true);

    ASSERT_NO_FATAL_FAILURE(displayServer.stop());
    ASSERT_EQ(displayServer.getIsRunning(), false);
}

TEST(DisplayServer, getIsRunning) {
    network::DisplayServer displayServer;

    ASSERT_EQ(typeid(displayServer.getIsRunning()), typeid(bool));
}

TEST(DisplayServer, getServerSocket) {
    network::DisplayServer displayServer;

    ASSERT_EQ(typeid(displayServer.getServerSocket()), typeid(network::Socket));
}


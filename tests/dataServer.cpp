// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

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

TEST(DataServer, getServerSocket) {
    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);

    ASSERT_EQ(typeid(dataServer.getServerSocket()), typeid(network::Socket));
}

TEST(DataServer, getDisplaySocket) {
    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);

    ASSERT_EQ(typeid(dataServer.getDisplaySocket()), typeid(network::Socket));
}

TEST(DataServer, getIsRunning) {
    std::string displayHost = "127.0.0.1";
    int displayPort = 9090;
    network::DataServer dataServer(displayHost, displayPort);

    ASSERT_EQ(typeid(dataServer.getIsRunning()), typeid(bool));
}
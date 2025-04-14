#include <gtest/gtest.h>
#include "socket.hpp"

TEST(Socket, create) {
    network::Socket socket;
    ASSERT_NO_FATAL_FAILURE(socket.create());
}

TEST(Socket, close) {
    network::Socket socket;
    socket.create();
    ASSERT_NO_FATAL_FAILURE(socket.close());
}
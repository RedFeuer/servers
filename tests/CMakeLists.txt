cmake_minimum_required(VERSION 3.16)
project(network_tests)

enable_testing()

set(CMAKE_CXX_STANDARD 20)


# Включение Google Test
find_package(GTest REQUIRED)
#include(GoogleTest)
include_directories(${GTEST_INCLUDE_DIRS})

# Тестовый исполняемый файл
add_executable(full_tests
        test_full.cpp
        test_servers.hpp
)

target_link_libraries(full_tests GTest::gtest GTest::gtest_main client data_server display_server)

add_test(NAME full_tests COMMAND full_tests)

# Добавление тестов в CTest
gtest_discover_tests(full_tests)
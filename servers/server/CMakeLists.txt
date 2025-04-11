cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 20)

add_library(server STATIC include/server.cpp)

target_include_directories(server PUBLIC source)

target_link_libraries(server PUBLIC socket)
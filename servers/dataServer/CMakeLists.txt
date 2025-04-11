cmake_minimum_required(VERSION 3.16)
project(data_server)

set(CMAKE_CXX_STANDARD 20)

add_library(data_server STATIC include/dataServer.cpp)

target_include_directories(data_server PUBLIC source)

target_link_libraries(data_server PUBLIC server socket)
cmake_minimum_required(VERSION 3.16)
project(display_server)

set(CMAKE_CXX_STANDARD 20)

add_library(display_server STATIC include/displayServer.cpp)

target_include_directories(display_server PUBLIC source)

target_link_libraries(display_server PUBLIC server socket)
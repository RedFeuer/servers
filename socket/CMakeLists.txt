cmake_minimum_required(VERSION 3.16)
project(socket)

set(CMAKE_CXX_STANDARD 20)

add_library(socket STATIC include/socket.cpp)

target_include_directories(socket PUBLIC source)

if(WIN32)
    target_link_libraries(socket PUBLIC ws2_32)
endif()

#add_dependencies(socket lib_name)
cmake_minimum_required(VERSION 3.16)
project(client)

set(CMAKE_CXX_STANDARD 20)

add_library(client STATIC include/client.cpp)

target_include_directories(client PUBLIC source)

if(WIN32)
    target_link_libraries(client PUBLIC ws2_32)
endif()

#add_dependencies(client lib_name)
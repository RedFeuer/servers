cmake_minimum_required(VERSION 3.16)
project(client)

set(CMAKE_CXX_STANDARD 20)

# Библиотека
add_library(client STATIC include/client.cpp)

target_include_directories(client PUBLIC source)

target_link_libraries(client PUBLIC server socket)

# Исполняемый файл
add_executable(client_exec main.cpp)

target_include_directories(client_exec PRIVATE include source)

target_link_libraries(client_exec PRIVATE client)

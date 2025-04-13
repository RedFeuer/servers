cmake_minimum_required(VERSION 3.16)
project(display_server)

set(CMAKE_CXX_STANDARD 20)

# Библиотека
add_library(display_server STATIC include/displayServer.cpp)

target_include_directories(display_server PUBLIC source)

target_link_libraries(display_server PUBLIC server socket)

# Исполняемый файл
add_executable(display_server_exec main.cpp)

target_include_directories(display_server_exec PRIVATE include source)

target_link_libraries(display_server_exec PRIVATE display_server)

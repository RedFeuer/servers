cmake_minimum_required(VERSION 3.16)
project(servers)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

add_subdirectory(server)
add_subdirectory(dataServer)
add_subdirectory(displayServer)

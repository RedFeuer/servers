cmake_minimum_required(VERSION 3.16)
project(network)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

find_package(nlohmann_json REQUIRED)

#if (TIMING_BUILD)
#    add_executable(Timing timing.cpp)
#    target_link_libraries(Timing game)
#endif()

add_subdirectory(servers)
add_subdirectory(socket)
add_subdirectory(client)
# Добавление исполняемого файла
#add_executable(network main.cpp)

if (TESTING_BUILD)
    add_subdirectory(tests)
endif ()
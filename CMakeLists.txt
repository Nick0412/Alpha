cmake_minimum_required(VERSION 3.20)
project(Alpha)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
enable_testing()

include(Common)

add_library(Networking STATIC
    "src/Networking/InMemoryNetwork.cpp"
    "src/Networking/IPv4Address.cpp"
    "src/Networking/Sockets.cpp")
target_include_directories(Networking PUBLIC "include")

parse_test_file(FILENAME "${CMAKE_SOURCE_DIR}/test/Networking/InMemoryNetworkTest.cpp" INCLUDES "include" LIBRARIES Networking)

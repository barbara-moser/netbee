cmake_minimum_required(VERSION 3.13)

project(netbee C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(dependencies)
add_subdirectory(contrib)
add_subdirectory(src)

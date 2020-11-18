cmake_minimum_required(VERSION 3.2)
PROJECT(netbee_example)
add_subdirectory(dependencies/netbee)

add_executable (packetdecoder packetdecoder.cpp)
target_link_libraries(packetdecoder PUBLIC netbee)

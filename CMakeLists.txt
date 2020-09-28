cmake_minimum_required(VERSION 3.12)
project(PortReadingCPP)

set(CMAKE_CXX_STANDARD 14)

add_executable(PortReadingCPP main.cpp Serial.cpp Serial.hpp)
cmake_minimum_required(VERSION 3.22)
project(resistive_network)

include_directories(include source)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "source/*.cpp" "include/*.h")

add_executable(code ${SOURCES} main.cpp)

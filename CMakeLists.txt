cmake_minimum_required(VERSION 3.22)
project(resistive_network)

set(CMAKE_CXX_STANDARD 14)

add_executable(resistive_network main.cpp matrix.cpp fraction.cpp fraction.h matrix.h resistive_network.cpp resistive_network.h)

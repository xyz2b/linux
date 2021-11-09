cmake_minimum_required(VERSION 3.19)
project(linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 14)

add_executable(linux main.cpp include/common.h thread/oop/JavaThread.cpp thread/oop/JavaThread.h)
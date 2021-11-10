cmake_minimum_required(VERSION 3.16.5)
project(linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 14)

add_executable(linux main.cpp include/common.h thread/oop/JavaThread.cpp thread/oop/JavaThread.h jvm.cpp jvm.h thread/oop/TaskPool.cpp thread/oop/TaskPool.h thread/oop/Task.cpp thread/oop/Task.h thread/oop/ThreadPool.cpp thread/oop/ThreadPool.h thread/oop/ManageThread.cpp thread/oop/ManageThread.h)
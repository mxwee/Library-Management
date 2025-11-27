cmake_minimum_required(VERSION 4.0)
project(Library)

set(CMAKE_CXX_STANDARD 20)

add_executable(Library main.cpp
        Queue.h
        Book.cpp
        Book.h
        Queue.cpp)

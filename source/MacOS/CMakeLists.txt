cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 20)

add_executable(src src.cpp heap.h heap.cpp tool.h tool.cpp unittest.h unittest.cpp)

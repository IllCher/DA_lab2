cmake_minimum_required(VERSION 3.10)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)

add_executable(benchmark main.cpp)
set (CMAKE_CXX_FLAGS "-pg")
add_executable(test_gprof test.cpp)
cmake_minimum_required(VERSION 3.8)
project(two_way)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Generator.cpp Generator.h Tester.cpp Tester.h)
add_executable(two_way ${SOURCE_FILES})
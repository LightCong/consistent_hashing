cmake_minimum_required(VERSION 3.7)
project(consistent_hashing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp consistent_hashing.hpp)
add_definitions(-DDEBUG)
add_executable(consistent_hashing ${SOURCE_FILES})

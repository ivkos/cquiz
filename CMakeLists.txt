cmake_minimum_required(VERSION 3.5)
project(cquiz)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES src/main.c)
add_executable(cquiz ${SOURCE_FILES})
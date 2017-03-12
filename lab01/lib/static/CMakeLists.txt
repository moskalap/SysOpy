cmake_minimum_required(VERSION 3.6)
project(static)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES library.c library.h)
add_library(static ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.6)
project(sys_op_tasks)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES lab01/lib/static/main.c)
add_executable(sys_op_tasks ${SOURCE_FILES})
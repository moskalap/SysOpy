cmake_minimum_required(VERSION 3.6)
project(sys_op_tasks)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES tests/lib/source/main.c tests/lib/source/dynamic.c moskala-przemyslaw/cw02/zad1/fio.c)
add_executable(sys_op_tasks ${SOURCE_FILES})
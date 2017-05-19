cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -fno-stack-protector)

project(cw8z1)
add_executable(cw8z1 moskala-przemyslaw/cw08/zad1/finder1.c)

project(file_creator)
add_executable(file_creator moskala-przemyslaw/cw08/zad1/file_creator.c)
target_link_libraries(cw8z1 pthread)

project(cw8z2)
add_executable(cw8z2 moskala-przemyslaw/cw08/zad2/threads_tester.c)
target_link_libraries(cw8z2 pthread)

project(finderver2)
add_executable(finderver2 moskala-przemyslaw/cw08/zad1/finder2.c)
target_link_libraries(finderver2 pthread)


project(finderver3)
add_executable(finderver3 moskala-przemyslaw/cw08/zad1/finder3.c)
target_link_libraries(finderver3 pthread)







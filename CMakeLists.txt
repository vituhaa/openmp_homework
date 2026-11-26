cmake_minimum_required(VERSION 3.10)
project(HW2)

find_package(OpenMP REQUIRED)

add_executable(first_task first_task.c)

target_link_libraries(first_task PRIVATE OpenMP::OpenMP_CXX)

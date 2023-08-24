cmake_minimum_required(VERSION 3.22)

project(snowflake C)

set(CMAKE_C_STANDARD 11)

aux_source_directory(./src DIR_SRCS)

add_executable(tests/main.c)

include_directories(test PUBLIC /usr/local/include ./include)
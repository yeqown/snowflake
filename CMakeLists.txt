cmake_minimum_required(VERSION 3.26)

project(snowflake C)

set(CMAKE_C_STANDARD 11)

aux_source_directory(./src DIR_SRCS)

add_library(
        snowflake_lib
        ${DIR_SRCS}
)

add_library(
        snowflake
        ${DIR_SRCS}
)

include_directories(./include)

add_subdirectory(tests)
add_subdirectory(cmd/snowflake)
add_subdirectory(contrib/lua)

set_target_properties(
        snowflake_lib
        PROPERTIES
        OUTPUT_NAME "snowflake"
)
cmake_minimum_required(VERSION 3.26)

project(snowflake C)

set(CMAKE_C_STANDARD 11)

aux_source_directory(./src DIR_SRCS)

add_library(
        snowflake_lib
        ${DIR_SRCS}
)

# Alias snowflake_lib to snowflake,
# so that dependency could link library using `snowflake`
add_library(
        snowflake ALIAS snowflake_lib
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
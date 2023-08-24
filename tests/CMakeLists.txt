add_executable(snowflake_test main.c ../src)

target_link_libraries(snowflake_test snowflake)
target_link_directories(snowflake_test PRIVATE ../src)
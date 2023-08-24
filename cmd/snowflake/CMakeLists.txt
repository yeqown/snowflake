add_executable(
        snowflake_cli
        ../../src
        main.c
)

install(
        TARGETS snowflake_cli
        RUNTIME DESTINATION bin
)

target_link_libraries(
        snowflake_cli
        snowflake
)

target_link_directories(
        snowflake_cli
        PRIVATE ../../src
)

set_target_properties(
        snowflake_cli
        PROPERTIES
        OUTPUT_NAME snowflake
)
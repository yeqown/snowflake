add_library(
        snowflake_lua
        SHARED
        lua_snowflake.c
)

if (APPLE)
    set(LUA_INCLUDE_DIR /opt/homebrew/opt/lua/include/lua)
    set(LUA_LIB_DIR /opt/homebrew/opt/lua/lib)
endif (APPLE)

if (UNIX AND NOT APPLE)
    set(LUA_INCLUDE_DIR /usr/include/lua)
    set(LUA_LIB_DIR /usr/lib/lua)
endif (UNIX AND NOT APPLE)

include_directories(${LUA_INCLUDE_DIR})

target_link_libraries(
        snowflake_lua
        snowflake
        lua
)
target_link_directories(
        snowflake_lua
        PRIVATE ../../src
        PUBLIC ${LUA_LIB_DIR}
)

set_target_properties(
        snowflake_lua
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "snowflake"
        SUFFIX ".so"
)
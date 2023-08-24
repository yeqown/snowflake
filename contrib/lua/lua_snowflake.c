/* Copyright (C) 2023
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "snowflake.h"

int l_snowflake_new(lua_State *L) {

}

int l_snowflake_parse(lua_State *L) {

}

int l_snowflake_worker_next_id(lua_State *L) {

}

int l_snowflake_worker_next_ids(lua_State *L) {

}

int luaopen_snowflake(lua_State *L) {
  luaL_Reg snowflake_lib[] = {
    {"new", l_snowflake_new},
    {"parse", l_snowflake_parse},
    {NULL, NULL}
  };

  luaL_newlib(L, snowflake_lib);

  // set snowflake as global variable
  lua_pushvalue(L, -1);
  lua_setglobal(L, "snowflake");

  return 1;
}
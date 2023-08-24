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
#include <stdlib.h>

#include "snowflake.h"

int l_snowflake_worker_next_id(lua_State *L);
int l_snowflake_worker_next_ids(lua_State *L);

/*
 * Usage in lua:
 * worker = snowflake.new(1?)
 */
int l_snowflake_new(lua_State *L) {
  // get the top value of the stack, if it's not empty and not an integer,
  // return error.
  uint64_t worker_id = 0;
  if (!lua_isnil(L, 1)) {
    if (!lua_isinteger(L, 1)) {
      lua_pushstring(L, "worker_id must be an integer");
      lua_error(L);
    }
    worker_id = (uint64_t) lua_tointegerx(L, 1, NULL);
  }
  if (worker_id > MAX_WORKER_ID) {
    lua_pushstring(L, "worker_id in range [0, 1023]");
    lua_error(L);
  }
  lua_pop(L, -1);

  lua_newtable(L);

  snowflake_Worker *worker = snowflake_NewWorker(worker_id);
  if (NULL == worker) {
    lua_pushstring(L, "failed to create snowflake worker");
    lua_error(L);
  }
  lua_pushlightuserdata(L, worker);

  luaL_Reg snowflake_worker_funcs[] = {
      {"next_id", l_snowflake_worker_next_id},
      {"next_ids", l_snowflake_worker_next_ids},
      {NULL, NULL},
  };

  luaL_setfuncs(L, snowflake_worker_funcs, 1);

  return 1;
}

/*
 * Usage in Lua:
 * snowflake.parse()
 */
int l_snowflake_parse(lua_State *L) {
  if (lua_isnil(L, -1) || !lua_isinteger(L, -1)) {
    lua_pushstring(L, "id must be an unsigned integer");
    lua_error(L);
  }

  uint64_t id = (uint64_t) lua_tointegerx(L, -1, NULL);
  if (id == 0) {
    lua_pushstring(L, "id must be an unsigned integer");
    lua_error(L);
  }
  lua_pop(L, -1);

  const snowflake_IDState *state = snowflake_ParseId((snowflake_ID) id);

  lua_newtable(L);
  lua_pushinteger(L, (long long) state->timestamp);
  lua_setfield(L, -2, "timestamp");

  lua_pushinteger(L, (long long) state->worker_id);
  lua_setfield(L, -2, "worker_id");

  lua_pushinteger(L, (long long) state->count);
  lua_setfield(L, -2, "count");

  return 1;
}

/*
 * Usage in lua:
 * local worker = snowflake.new(1);
 * local id = worker.next_id()
 */
int l_snowflake_worker_next_id(lua_State *L) {
  snowflake_Worker *worker = (snowflake_Worker *) lua_touserdata(L, lua_upvalueindex(1));
  if (NULL == worker) {
    lua_pushstring(L, "worker is null");
    lua_error(L);
  }

  snowflake_ID id = snowflake_NextId(worker, true);
  if (id == 0) {
    lua_pushstring(L, "failed to generate snowflake id");
    lua_error(L);
  }

  lua_pushinteger(L, (long long) id);

  return 1;
}

/*
 * Usage in Lua:
 * local worker = snowflake.new(1);
 */
int l_snowflake_worker_next_ids(lua_State *L) {
  if (lua_isnil(L, -1) || !lua_isinteger(L, -1)) {
    lua_pushstring(L, "count must be an unsigned integer");
    lua_error(L);
  }

  uint64_t count = (uint64_t) lua_tointegerx(L, -1, NULL);
  if (count == 0) {
    lua_pushstring(L, "count cannot be zero");
    lua_error(L);
  }
  lua_pop(L, -1);

  snowflake_Worker *worker = (snowflake_Worker *) lua_touserdata(L, lua_upvalueindex(1));
  if (NULL == worker) {
    lua_pushstring(L, "worker is null");
    lua_error(L);
  }

  snowflake_ID *ids = malloc(sizeof(snowflake_ID) * count);
  if (snowflake_NextIds(worker, count, ids, false) <= 0) {
    lua_pushstring(L, "failed to generate snowflake ids");
    lua_error(L);
  }

  lua_newtable(L);
  for (uint64_t i = 0; i < count; i++) {
    lua_pushinteger(L, (long long) ids[i]);
    lua_rawseti(L, -2, i + 1);
  }

  free(ids);

  return 1;
}

int luaopen_snowflake(lua_State *L) {
  luaL_Reg snowflake_funcs[] = {
      {"new", l_snowflake_new},
      {"parse", l_snowflake_parse},
      {NULL, NULL}
  };

  luaL_newlib(L, snowflake_funcs);

  lua_pushliteral(L, "yeqown");
  lua_setfield(L, -2, "_AUTHOR");

  lua_pushliteral(L, "lua-snowflake");
  lua_setfield(L, -2, "_NAME");

  lua_pushliteral(L, "0.1.0");
  lua_setfield(L, -2, "_VERSION");

  // set snowflake as global variable
  lua_pushvalue(L, -1);
  lua_setglobal(L, "snowflake");

  return 1;
}
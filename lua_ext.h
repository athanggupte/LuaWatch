#pragma once

#include "lua_fwd.h"

int luaL_openluaext(lua_State* L);

int luaL_loadnamedstringx(lua_State* L, const char* s, const char* name);
#define luaL_loadnamedstring(L, s) luaL_loadnamedstringx(L, s, #s)
#define luaL_loadnamedlstring(L, s, sz) luaL_loadbuffer(L, s, sz, #s)
#define luaL_donamedstring(L, s) (luaL_loadnamedstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define luaL_donamedlstring(L, s, sz) (luaL_loadnamedlstring(L, s, sz) || lua_pcall(L, 0, LUA_MULTRET, 0))
int lua_loadnamed(lua_State* L);

void lua_check(lua_State* L, int retval, const char* tag);

void print_topx(lua_State* L, int lineno, const char* filename);
#define print_top(L) print_topx(L, __LINE__, __FILE__)

void dumptable(lua_State * L, int index);


#include "lua_ext.h"

#include <string.h>

int luaL_loadnamedstringx(lua_State* L, const char* s, const char* name)
{
	return luaL_loadbuffer(L, s, strlen(s), name);
}

int lua_loadnamed(lua_State* L)
{
	const char* script = luaL_checkstring(L, -2);
    const char* name = luaL_checkstring(L, -1);
	if (luaL_loadnamedstringx(L, script, name) != LUA_OK)
	{
		lua_pushnil(L);
		lua_insert(L, -2);
		return 2;
	}
	return 1;
}

int luaL_openluaext(lua_State* L)
{
	lua_pushcfunction(L, lua_loadnamed);
	lua_setglobal(L, "loadnamed");

	return LUA_OK;
}

void lua_check(lua_State* L, int retval, const char* tag)
{
	if ((retval) != LUA_OK)
	{
		const char* errormsg = lua_tostring(L, -1);
		printf("[%s] Lua Error: %s\n", tag ? tag : "lua", errormsg);
	}
}

void print_topx(lua_State* L, int lineno, const char* filename)
{
    printf("[%s:%d] > top : %d\n", filename, lineno, lua_gettop(L));
}

void dumptable(lua_State * L, int index)
{
	lua_pushnil(L);
	(void)((index < 0) ? --index : index);
	while (lua_next(L, index) != 0)
	{
		printf("  [%s] : (%s)\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
		lua_pop(L, 1);
	}
}

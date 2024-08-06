#include "lwlua.h"

#include "lwdebug.h"
#include "lua_ext.h"
#include "hashmap.h"

#include <stdio.h>
#include <string.h>


/************* Lua API **************/
int lua_openluawatch(lua_State* L);
int lua_debugger_load(lua_State * L);
int lua_debugger_isloaded(lua_State * L);
int lua_debugger_start(lua_State * L);
int lua_debugger_set_breakpoint(lua_State * L);
int lua_debugger_delete_breakpoint(lua_State * L);

int register_debugger(lua_State * L);
/*************************************/

static lw_Debugger * get_debugger_from_state(lua_State * L)
{
	luaL_getmetatable(L, "lw_Debugger");
	lua_getfield(L, -1, "__udata");
    lw_Debugger* debugger = (lw_Debugger*)luaL_checkudata(L, -1, "lw_Debugger");
	lua_pop(L, 2);
	return debugger;
}

int lua_openluawatch(lua_State* L)
{
	luaL_openluaext(L);
	register_debugger(L);

	return LUA_OK;
}

int lua_debugger_load(lua_State * L)
{
    lw_Debugger* debugger = get_debugger_from_state(L);
	debugger->chunkref = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

int lua_debugger_isloaded(lua_State * L)
{
	lw_Debugger* debugger = get_debugger_from_state(L);
	lua_pushboolean(L, (debugger->chunkref != LUA_NOREF));
	return 1;
}

int lua_debugger_start(lua_State * L)
{
	lw_Debugger* debugger = get_debugger_from_state(L);
	lw_start_session(debugger, L);
	return 0;
}

int lua_debugger_set_breakpoint(lua_State * L)
{
	lw_Debugger* debugger = get_debugger_from_state(L);

	const char* chunkname = luaL_checkstring(L, -2);
	int line = luaL_checkinteger(L, -1);

	lw_add_breakpoint(debugger, &(lw_Breakpoint){ .chunkname=chunkname, .line=line });
	return 0;
}

int lua_debugger_delete_breakpoint(lua_State * L)
{
	lw_Debugger* debugger = get_debugger_from_state(L);

	const char* chunkname = luaL_checkstring(L, -2);
	int line = luaL_checkinteger(L, -1);

	lw_delete_breakpoint(debugger, &(lw_Breakpoint){ .chunkname=chunkname, .line=line });
	return 0;
}

int register_debugger(lua_State * L)
{
	lw_Debugger * debugger = (lw_Debugger*)lua_newuserdata(L, sizeof(lw_Debugger));
	lw_debugger_init(debugger, L);

	luaL_newmetatable(L, "lw_Debugger");

	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	lua_pushvalue(L, -2);
	lua_setfield(L, -2, "__udata");

	luaL_Reg funcs[] = {
		{ "load", lua_debugger_load },
		{ "isloaded", lua_debugger_isloaded },
		{ "start", lua_debugger_start },
		{ "set_breakpoint", lua_debugger_set_breakpoint },
		{ "delete_breakpoint", lua_debugger_delete_breakpoint },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, funcs, 0);

	lua_setmetatable(L, -2);

	return 1;
}

LUAWATCH_API int luaopen_lwlua(lua_State *L)
{
	lua_openluawatch(L);
	return 1;
}

#include "lwdebug.h"
#include "lua_ext.h"

int default_hook(lw_Debugger * debugger, lua_State * L, lw_DebugData * dat)
{
	printf("[default_hook] called\n");
}

int main()
{
	lua_State * L = luaL_newstate();

	luaL_openlibs(L);

	lw_Debugger debugger;
	lw_Debugger * pDebugger = &debugger;
	lw_debugger_init(pDebugger, L);

	lua_check(L, luaL_loadfile(L, "test_script.lua"), "load script");

	lua_Debug ar;
	lua_getinfo(L, ">L", &ar);
	
	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		printf("  [%lld] : %lld\n", lua_tointeger(L, -2), lua_tointeger(L, -1));
		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	lw_load_chunk(pDebugger, L);
	lw_debugger_setcallback(pDebugger, default_hook, -1);
	lw_add_breakpoint(pDebugger, &(lw_Breakpoint){ .chunkname="test_script.lua", .line=30 });

	lw_start_session(pDebugger, L);

	return 0;
}

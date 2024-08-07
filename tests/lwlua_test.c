#include "lwdebug.h"
#include "lwlua.h"
#include "lua_ext.h"

#include <stdio.h>

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

int main()
{
	lua_State * L = luaL_newstate();

	luaL_openlibs(L);
	luaopen_lwlua(L);

	print_top(L);
	luaL_dostring(L, "function doSomething() print \"doing something...\" end");
	print_top(L);

	if (0) { // Check lw_Debugger metatable
		lua_check(L, luaL_dostring(L, "return function(tab) print(tab) for k, v in pairs(tab) do print(string.format(\"[%s]: %s\", tostring(k), tostring(v))) end end"), "print_table");
		lua_check(L, luaL_dostring(L, "return getmetatable(debugger)"), "get_metatable");
		lua_check(L, lua_pcall(L, 1, 0, 0), "pcall");
	}
    
    lua_getglobal(L, "debugger");
    lua_getfield(L, -1, "load");
	lua_check(L, luaL_dostring(L, "script = \"local a = 10 local b = 32 print(a + b)\" local func = loadnamed(script, \"script\") return func"), "load_script");
    lua_check(L, lua_pcall(L, 1, 0, 0), "pcall debugger.load");

	return 0;

	int exist;

	lw_Debugger debugger; // create on Lua stack as userdata as part of luawatch module
	lw_debugger_init(&debugger, L);

	lw_add_breakpoint(&debugger, &(lw_Breakpoint){ .line=10, .chunkname="sample.lua" });
	lw_add_breakpoint(&debugger, &(lw_Breakpoint){ .line=15, .chunkname="sample.lua" });

	exist = lw_check_breakpoint(&debugger, &(lw_Breakpoint){ .line=10, .chunkname="sample.lua" });
	printf("%s\n", exist ? "FOUND" : "NOT FOUND");

	lw_delete_breakpoint(&debugger, &(lw_Breakpoint){ .line=10, .chunkname="sample.lua" });

	exist = lw_check_breakpoint(&debugger, &(lw_Breakpoint){ .line=10, .chunkname="sample.lua" });
	printf("%s\n", exist ? "FOUND" : "NOT FOUND");

	return 0;
}

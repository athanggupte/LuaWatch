#include "lwdebug.h"
#include "hashmap.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

/********************** lw_Breakpoint **********************/
int breakpoint_compare(const void* a, const void* b, void* udata)
{
	const lw_Breakpoint* bpa = a;
	const lw_Breakpoint* bpb = b;
	return (bpa->line - bpb->line) && strcmp(bpa->chunkname, bpb->chunkname);
}

static uint64_t chunkname_hash(const void* item, uint64_t seed0, uint64_t seed1)
{
	const lw_Breakpoint* bp = item;
	return hashmap_xxhash3(bp->chunkname, strlen(bp->chunkname), seed0, seed1);
}

static uint64_t line_hash(const void* item, uint64_t seed0, uint64_t seed1)
{
	const lw_Breakpoint* bp = item;
	return hashmap_xxhash3(&bp->line, sizeof(bp->line), seed0, seed1);
}

static uint64_t breakpoint_hash(const void* item, uint64_t seed0, uint64_t seed1)
{
	return chunkname_hash(item, seed0, seed1) ^ line_hash(item, seed0, seed1);
}
/***********************************************************/

// internal
typedef struct _RBNode _RBNode;
struct _RBNode
{
	lw_Breakpoint bp;
	_RBNode *left, *right, *parent;
	int color; // 0 for red, 1 for black
};

/********************* lw_BreakpointSet ********************/
void lw_breakpointset_init(lw_BreakpointSet * bpset)
{
	bpset->map = hashmap_new(sizeof(lw_Breakpoint), 8, 0, 0, breakpoint_hash, breakpoint_compare, NULL, NULL);
}

void lw_breakpointset_add(lw_BreakpointSet * bpset, const lw_Breakpoint * bp)
{
	(void)hashmap_set(bpset->map, bp);
}

const lw_Breakpoint * lw_breakpointset_get(lw_BreakpointSet * bpset, const lw_Breakpoint * bp)
{
	return hashmap_get(bpset->map, bp);
}

const lw_Breakpoint * lw_breakpointset_delete(lw_BreakpointSet * bpset, const lw_Breakpoint * bp)
{
	return hashmap_delete(bpset->map, bp);
}

void lw_breakpointset_destroy(lw_BreakpointSet * bpset)
{
	hashmap_free(bpset->map);
}
/***********************************************************/

/*********************** lw_Debugger ***********************/
void lw_debugger_init(lw_Debugger * debugger, lua_State * L)
{
	lw_breakpointset_init(&debugger->breakpoints);
	debugger->chunkref = LUA_NOREF;
	lua_pushstring(L, "__lw_Debugger");
	lua_pushlightuserdata(L, debugger);
	lua_rawset(L, LUA_REGISTRYINDEX);
}

void lw_debugger_destroy(lw_Debugger * debugger, lua_State * L)
{
	lw_breakpointset_destroy(&debugger->breakpoints);
	((debugger->chunkref != LUA_NOREF) ? ((void)0) : luaL_unref(L, LUA_REGISTRYINDEX, debugger->chunkref));
	lua_pushstring(L, "__lw_Debugger");
	lua_pushnil(L);
	lua_rawset(L, LUA_REGISTRYINDEX);
}

size_t _lw_debugger_size()
{
	return sizeof(lw_Debugger);
}

void lw_add_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp)
{
	(void)lw_breakpointset_add(&debugger->breakpoints, bp);
}

const lw_Breakpoint * lw_delete_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp)
{
	return lw_breakpointset_delete(&debugger->breakpoints, bp);
}

int lw_check_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp)
{
	return !!lw_breakpointset_get(&debugger->breakpoints, bp);
}

static void debug_hook(lua_State * L, lua_Debug * ar)
{
	lua_pushstring(L, "__lw_Debugger");
	lua_rawget(L, LUA_REGISTRYINDEX);
	lw_Debugger * debugger = lua_touserdata(L, -1);
	
	lw_DebugData data = { .ar=ar, .event=ar->event };

	int event = ar->event;
	switch (event)
	{
	case LUA_HOOKCALL: break; // enter function
	case LUA_HOOKRET: break; // exit function
	case LUA_HOOKLINE: 
	{
		int line = ar->currentline;
		int ret = 0;
		lua_getinfo(L, "S", ar);

		if (lw_check_breakpoint(debugger, &(lw_Breakpoint){ .chunkname=ar->short_src, .line=line }))
		{
			data.event = lw_KBreakpoint;
			if (debugger->_hookmask != 0) // check more mask conditions here
				ret = debugger->_cb(debugger, L, &data);
		}

		break;
	}
	case LUA_HOOKCOUNT: break;
	case LUA_HOOKTAILCALL: break;
	}
}

int lw_load_chunk(lw_Debugger * debugger, lua_State * L)
{
	if (!lua_isfunction(L, -1)) {
		printf("[Error] Expected a function!\n");
		return -1;
	}
	debugger->chunkref = luaL_ref(L, LUA_REGISTRYINDEX);

	return LUA_OK;
}

int lw_start_session(lw_Debugger * debugger, lua_State * L)
{
	assert(debugger->chunkref != LUA_NOREF);
	lua_sethook(L, debug_hook, LUA_MASKLINE, 0);

	lua_rawgeti(L, LUA_REGISTRYINDEX, debugger->chunkref);
	lua_pcall(L, 0, LUA_MULTRET, 0);

	return LUA_OK;
}

void lw_debugger_setcallback(lw_Debugger * debugger, lw_DebugCallback hook, int mask)
{
	debugger->_hookmask = mask;
	debugger->_cb = hook;
}

/***********************************************************/
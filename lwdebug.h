#pragma once
#include "lwdef.h"
#include "lua_fwd.h"
#include <stdint.h>

typedef struct hashmap hashmap;

/************************ Public API ***********************/
enum lw_Event
{
	lw_KCall = 0,
	lw_KReturn,
	lw_KLine,
	lw_KCount,
	lw_KTailCall,
	lw_KBreakpoint,
};

enum lw_Mode
{
	lw_KContinue = 0, // stop only on breakpoint
	lw_KStepOver, // stop on line; if call then don't stop until return or breakpoint
	lw_KStepInto, // stop on everything
	lw_KStepOut, // stop only on return or breakpoint
};

DEFINE_TYPE(lw_Breakpoint);
DEFINE_TYPE(lw_BreakpointSet);
DEFINE_TYPE(lw_Debugger);
DEFINE_TYPE(lw_DebugData);

typedef int (*lw_DebugCallback) (lw_Debugger * debugger, lua_State * L, lw_DebugData * dat);

struct lw_Breakpoint
{
	int line;
	const char * chunkname;
};

struct lw_BreakpointSet
{
	hashmap * map;
	void * tree;
};

void 	lw_breakpointset_init(lw_BreakpointSet * bpset);
void 	lw_breakpointset_destroy(lw_BreakpointSet * bpset);
void 	lw_breakpointset_add(lw_BreakpointSet * bpset, const lw_Breakpoint * bp);
const lw_Breakpoint * 	lw_breakpointset_get(lw_BreakpointSet * bpset, const lw_Breakpoint * bp);

struct lw_Debugger
{
	lw_BreakpointSet breakpoints;
	lw_DebugCallback _cb;
	int chunkref;
	int _hookmask;

	/* private */
#ifndef NDEBUG
	int verbose;

	#define log_verbose(fmt, ...) printf("[LuaWatch] :: ")
#endif
};

void 	lw_debugger_init(lw_Debugger * debugger, lua_State * L);
void 	lw_debugger_destroy(lw_Debugger * debugger, lua_State * L);

void 	lw_add_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp);
const lw_Breakpoint * 	lw_delete_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp);
int 	lw_check_breakpoint(lw_Debugger * debugger, const lw_Breakpoint * bp);

int 	lw_load_chunk(lw_Debugger * debugger, lua_State * L);
int 	lw_start_session(lw_Debugger * debugger, lua_State * L);

// TODO: Add callbacks here to control flow
//     [1] commandline -> ./ldb
//     [2] gui -> C# or ImGui (seems simpler to get started)
//     [2] remote -> JSON (simdjson)
//     [2] lua -> require "luawatch"

struct lw_DebugData
{
	lua_Debug * ar;
	int event; // lw_Event
	int nret; // valid for RET events
};

void lw_debugger_setcallback(lw_Debugger * debugger, lw_DebugCallback cb, int mask);

// on breakpoint
// on 

/**********************************/
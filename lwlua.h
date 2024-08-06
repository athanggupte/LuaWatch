#pragma once
#include "lua_fwd.h"
#include <stdint.h>

#ifndef LUAWATCH_API
#ifdef _WIN32
#define LUAWATCH_API __declspec(dllexport)
#else
#define LUAWATCH_API __attribute__ ((visibility ("default")))
#endif
#endif

LUAWATCH_API int luaopen_lwlua(lua_State *L);


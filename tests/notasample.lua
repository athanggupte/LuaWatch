local luawatch = require "luawatch"

local chunk = loadfile("test_script.lua")
pcall(chunk)

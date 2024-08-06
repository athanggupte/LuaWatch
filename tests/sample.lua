local luawatch = require "luawatch"

local chunk = loadfile("test_script.lua")
luawatch.load(chunk)
luawatch.start()

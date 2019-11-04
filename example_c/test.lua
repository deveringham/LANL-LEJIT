--[[
	This is a Lua configuaration file generated with LEJITReader
	Edit any parameter here and rerrun the calling code without recompilation to
		see altered behavior.
	All mathematical and plotting functions of torch are enabled.
--]]

-- Include LuaJIT FFI Library
local ffi = require 'ffi'

-- Include torch libraries
local th = require 'torch'

testint = 42

testfunc = function(arg0, arg1)
	print(arg0 + arg1)
end

testarrfunc = function(arg0, arg1)
	-- Add a Lua function definition here to override the default behavior
end

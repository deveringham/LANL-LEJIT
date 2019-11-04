--
-- 	 _____     ________     _____  _____  _________  
--	|_   _|   |_   __  |   |_   _||_   _||  _   _  | 
-- 	  | |       | |_ \_|     | |    | |  |_/ | | \_| 
-- 	  | |   _   |  _| _  _   | |    | |      | |     
--	 _| |__/ | _| |__/ || |__' |   _| |_    _| |_    
--	|________||________|`.____.'  |_____|  |_____|   
--                                                 
--			  Lua Easy Just In Time Library
--						Version 1.0
--			  Los Alamos National Laboratory
--
-- Dylan Everingham 08/26/2016
-- performancetest_ffi.lua
--
-- Performance tests for LuaJIT FFI
-- Run with "luajit performancetest_ffi.lua"
--

local ffi = require ("ffi")

--[[ CONSTANTS ]]--

SEED = 123
N = 100
MC_ITERATIONS = 1000000
SOR_ITERATIONS = 10000
OMEGA = 1.25

-- Load functions to be called with FFI only once
perf = ffi.load("libperformancetest.so")

-- Declarations of C functions to be called with FFI
ffi.cdef[[
	double LU();
]]
ffi.cdef[[
	double MonteCarlo();
]]
ffi.cdef[[
	double SOR();
]]

--[[ LU DECOMPOSITION ]]--


function lua_lu_ffi()
	local time = os.clock()
	res = perf.LU()
	time = os.clock() - time
	print(string.format("lua_lu_ffi: %f", time))
	return res
end


--[[ MONTE CARLO ]]--

function lua_montecarlo_ffi()
	local time = os.clock()
	res = perf.MonteCarlo()
	time = os.clock() - time
	print(string.format("lua_montecarlo_ffi: %f", time))
	return res
end


--[[ SOR ]]--

function lua_sor_ffi()
	local time = os.clock()
	res = perf.SOR()
	time = os.clock() - time
	print(string.format("lua_sor_ffi: %f", time))
	return res
end


--[[ MAIN THREAD ]]--

print("----- LuaJIT FFI scientific benchmark tests -----")
lua_lu_ffi()
lua_montecarlo_ffi()
lua_sor_ffi()


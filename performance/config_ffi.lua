--[[							   ]]--
--[[ PERFORMANCE TESTING FUNCTIONS ]]--
--[[							   ]]--

local ffi = require ("ffi")

--[[ CONSTANTS ]]--

SEED = 123
N = 100
MC_ITERATIONS = 1000000
SOR_ITERATIONS = 10000
OMEGA = 1.25
REPS = 2

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

-- Load functions to be called with FFI only once
perf = ffi.load("libperformancetest.so")


--[[ LU DECOMPOSITION ]]--

function lua_lu_ffi()
	return perf.LU()
end


--[[ MONTE CARLO ]]--

function lua_montecarlo_ffi()
	return perf.MonteCarlo()
end


--[[ SOR ]]--

function lua_sor_ffi()
	return perf.SOR()
end
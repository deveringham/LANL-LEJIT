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
-- performancetest_insitu.lua
--
-- Performance tests for LuaJIT with insitu vis
-- Run with "luajit performancetest_insitu.lua"
--

require 'torch'
require 'gnuplot'


--[[ CONSTANTS ]]--

SEED = 123
MC_ITERATIONS = 1000000


--[[ MONTE CARLO ]]--

function lua_montecarlo_insitu()

	local time = os.clock()

	math.randomseed(SEED)

	under_curve = 0

	curve_x = torch.linspace(0, 1)
	curve_y = torch.sqrt(torch.ones(100):csub(curve_x:pow(2)))

	points_x = {}
	points_y = {}

	for count = 1, MC_ITERATIONS do
		x = math.random()
		y = math.random()

		table.insert(points_x, x)
		table.insert(points_y, y)

		if count % 10 == 0 then
			gnuplot.plot({curve_x,curve_y,'-'},{torch.Tensor(points_x),torch.Tensor(points_y),'+'})
		end

		if (x*x + y*y <= 1.0) then
			under_curve = under_curve + 1	
		end
	end

	time = os.clock() - time
	print(string.format("lua_montecarlo_insitu: %f", time))

	return ((under_curve / MC_ITERATIONS) * 4.0)
end


--[[ MAIN THREAD ]]--

print("----- LuaJIT scientific benchmark tests -----")
lua_montecarlo_insitu()

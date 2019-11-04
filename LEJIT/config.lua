--[[
	This is a Lua configuaration file generated with LEJITReader
	Edit any parameter here and rerrun the calling code without recompilation to
		see altered behavior.
--]]

--[[
A test std::function which takes an array argument
--]]
arr_func = function(arg0, arg1)
	-- Add a Lua function definition here to override the default behavior
end

--[[
A test callback std::function
--]]
do_calc = function(arg0, arg1)
	return arg0 + arg1
end

--[[
Nout: The number of steps performed between each output data point.
--]]
nout = 5

--[[
Numrevs: the number of revolutions around the Sun to simulate
--]]
numrevs = 10

--[[
Planetnum: number of planet to simulate i.e. 1 = Mercury, 2 = Venus, etc.
--]]
planetnum = 4

testarr = {4, 2, 42}

testarr2d = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}

testarr3d = {{{1.110000, 1.120000}, {1.210000, 1.220000}}, {{2.110000, 2.120000}, {2.210000, 2.220000}}}

teststring = "Hello World!"

testvec = {1, 2, 3}


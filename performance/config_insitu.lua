--[[							   ]]--
--[[ PERFORMANCE TESTING FUNCTIONS ]]--
--[[							   ]]--

require 'torch'
require 'gnuplot'

--[[ CONSTANTS ]]--

SEED = 123
N = 100
MC_ITERATIONS = 1000
SOR_ITERATIONS = 10000
OMEGA = 1.25


--[[ MONTE CARLO ]]--

function lua_montecarlo_insitu()

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

	return ((under_curve / MC_ITERATIONS) * 4.0)
end
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
-- performancetest.lua
--
-- Performance tests for LuaJIT
-- Run with "luajit performancetest.lua"
--

-- Uncomment these to impose a 1ms JIT time performance hit (each)
--require 'torch'
--require 'gnuplot'


--[[ CONSTANTS ]]--

SEED = 123
N = 100
MC_ITERATIONS = 1000000
SOR_ITERATIONS = 10000
OMEGA = 1.25
REPS = 2


--[[ BENCHMARK DEFINITIONS ]]--

function lua_montecarlo()
	local time = os.clock()

	math.randomseed(SEED)

	under_curve = 0

	for count = 1, MC_ITERATIONS do
		x = math.random()
		y = math.random()

		if (x*x + y*y <= 1.0) then
			under_curve = under_curve + 1
		end
	end

	time = os.clock() - time
	print(string.format("lua_montecarlo: %f", time))

	return ((under_curve / MC_ITERATIONS) * 4.0)
end

function lua_montecarlo_reps()
	local time = os.clock()
	res = 0

		for r = 1, REPS do

		math.randomseed(SEED)

		under_curve = 0

		for count = 1, MC_ITERATIONS do
			x = math.random()
			y = math.random()

			if (x*x + y*y <= 1.0) then
				under_curve = under_curve + 1
			end
		end

		res = ((under_curve / MC_ITERATIONS) * 4.0)
	end

	time = os.clock() - time
	print(string.format("lua_montecarlo repeated %d times: %f", REPS, time))

	return res
end

function lua_lu()
	local time = os.clock()

	pivot = {}
	math.randomseed(SEED)

	-- Generate a random matrix
	A = {}
	for i = 1, N do
		A[i] = {}
		for j = 1, N do
			A[i][j] = math.random()
		end
	end

	for j = 1, N do

		jp = j

		t = math.abs(A[j][j])
		for i = j + 1, N do
			ab = math.abs(A[i][j])
			if (ab > t) then
				jp = i
				t = ab
			end
		end

		pivot[j] = jp

		if (A[jp][j] == 0) then
			print("Zero pivot\n")
			return
		end

		if (jp ~= j) then
			tA = A[j]
			A[j] = A[jp]
			A[jp] = tA
		end

		if (j < N - 1) then
			recp = 1.0 / A[j][j]
			for k = j + 1, N do
				A[k][j] = A[k][j] * recp
			end
		end

		if (j < N - 1) then
			for ii = j + 1, N do
				Aii = A[ii]
				Aj = A[j]
				AiiJ = Aii[j]
				for jj = j + 1, N do
					Aii[jj] = Aii[jj] - (AiiJ * Aj[jj])
				end
			end
		end
	end

	-- return sum of diagonals
	sum = 0
	for i = 1, N do
		sum = sum + A[i][i]
	end

	time = os.clock() - time
	print(string.format("lua_lu: %f", time))

	return sum
end

function lua_lu_reps()
	local time = os.clock()
	sum = 0

	for r = 1, REPS do
		pivot = {}
		math.randomseed(SEED)

		-- Generate a random matrix
		A = {}
		for i = 1, N do
			A[i] = {}
			for j = 1, N do
				A[i][j] = math.random()
			end
		end

		for j = 1, N do

			jp = j

			t = math.abs(A[j][j])
			for i = j + 1, N do
				ab = math.abs(A[i][j])
				if (ab > t) then
					jp = i
					t = ab
				end
			end

			pivot[j] = jp

			if (A[jp][j] == 0) then
				print("Zero pivot\n")
				return
			end

			if (jp ~= j) then
				tA = A[j]
				A[j] = A[jp]
				A[jp] = tA
			end

			if (j < N - 1) then
				recp = 1.0 / A[j][j]
				for k = j + 1, N do
					A[k][j] = A[k][j] * recp
				end
			end

			if (j < N - 1) then
				for ii = j + 1, N do
					Aii = A[ii]
					Aj = A[j]
					AiiJ = Aii[j]
					for jj = j + 1, N do
						Aii[jj] = Aii[jj] - (AiiJ * Aj[jj])
					end
				end
			end
		end

		-- return sum of diagonals
		sum = 0
		for i = 1, N do
			sum = sum + A[i][i]
		end
	end

	time = os.clock() - time
	print(string.format("lua_lu repeated %d times: %f", REPS, time))

	return sum
end


--[[ SOR ]]--

function lua_sor()
	local time = os.clock()

	math.randomseed(SEED)

	-- Generate a random matrix
	G = {}
	for i = 1, N do
		G[i] = {}
		for j = 1, N do
			G[i][j] = math.random()
		end
	end

	local omega_over_four, one_minus_omega = OMEGA * 0.25, 1.0 - OMEGA

	Nm1 = N - 1

	for p = 1, SOR_ITERATIONS do
		for i = 2, Nm1 do
			local Gi, Gim1, Gip1 = G[i], G[i-1], G[i+1]
			for j = 2, Nm1 do
				Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] + Gi[j+1]) + one_minus_omega * Gi[j]
			end
		end
	end

		-- return sum of diagonals
	sum = 0
	for i = 1, N do
		sum = sum + G[i][i]
	end

	time = os.clock() - time
	print(string.format("lua_sor: %f", time))

	return sum
end

function lua_sor_reps()
	local time = os.clock()
	sum = 0
	for r = 1, REPS do
		math.randomseed(SEED)

		-- Generate a random matrix
		G = {}
		for i = 1, N do
			G[i] = {}
			for j = 1, N do
				G[i][j] = math.random()
			end
		end

		local omega_over_four, one_minus_omega = OMEGA * 0.25, 1.0 - OMEGA

		Nm1 = N - 1

		for p = 1, SOR_ITERATIONS do
			for i = 2, Nm1 do
				local Gi, Gim1, Gip1 = G[i], G[i-1], G[i+1]
				for j = 2, Nm1 do
					Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] + Gi[j+1]) + one_minus_omega * Gi[j]
				end
			end
		end

		sum = 0
		for i = 1, N do
			sum = sum + G[i][i]
		end
	end

	time = os.clock() - time
	print(string.format("lua_sor repeated %d times: %f", REPS, time))

	return sum
end


--[[ MAIN THREAD ]]--

print("----- LuaJIT scientific benchmark tests -----")
lua_lu()
lua_lu_reps()
lua_montecarlo()
lua_montecarlo_reps()
lua_sor()
lua_sor_reps()


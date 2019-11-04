--[[							   ]]--
--[[ PERFORMANCE TESTING FUNCTIONS ]]--
--[[							   ]]--

--[[ SQUARE ]]--

function lua_square(val)
	return val * val
end

function lua_square_loop_inc(num_iterations)
	for i=1, num_iterations do
		ret = i * i
	end
end

function lua_square_loop_same(num_iterations)
	ret = 0
	for i=1, num_iterations do
		ret = ret + 100 * 100
	end
	return ret
end


--[[ CONSTANTS ]]--

SEED = 123
N = 100
MC_ITERATIONS = 1000000
SOR_ITERATIONS = 10000
OMEGA = 1.25

--[[ LU DECOMPOSITION ]]--

function lua_multsub(x, y, z)
	return x - (y * z)
end

function lua_lu()
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
	return sum
end


--[[ MONTE CARLO ]]--

function lua_srand(seed)
	math.randomseed(seed)
end

function lua_rand()
	return math.random()
end

function lua_montecarlo()

	math.randomseed(SEED)

	under_curve = 0

	for count = 1, MC_ITERATIONS do
		x = math.random()
		y = math.random()

		if (x*x + y*y <= 1.0) then
			under_curve = under_curve + 1
		end
	end

	return ((under_curve / MC_ITERATIONS) * 4.0)
end


--[ SOR ]--

function lua_sorinner(j, omega_over_four, one_minus_omega, Gi, Gim1, Gip1)
	Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] + Gi[j+1]) + one_minus_omega * Gi[j]
end

function lua_sor1loop(omega_over_four, one_minus_omega, Gi, Gim1, Gip1)
	Nm1 = N - 1
	for j = 2, Nm1 do
		Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] + Gi[j+1]) + one_minus_omega * Gi[j]
	end
end

function lua_sor()
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
	return sum
end

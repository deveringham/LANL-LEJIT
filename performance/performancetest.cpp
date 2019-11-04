/*
 * 	 _____     ________     _____  _____  _________  
 *	|_   _|   |_   __  |   |_   _||_   _||  _   _  | 
 * 	  | |       | |_ \_|     | |    | |  |_/ | | \_| 
 * 	  | |   _   |  _| _  _   | |    | |      | |     
 *	 _| |__/ | _| |__/ || |__' |   _| |_    _| |_    
 *	|________||________|`.____.'  |_____|  |_____|   
 *                                                 
 *			  Lua Easy Just In Time Library
 *						Version 1.0
 *			  Los Alamos National Laboratory
 *
 * Dylan Everingham 08/26/2016
 * performancetest.cpp
 *
 * Performance tests for C++ and LEJIT
 * To run, simply make and ./performancetest
 * Can change test parameters below and comment out unwanted tests
 * in the main thread if desired
 *
 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../LEJIT/Lejit.hpp"

/* Benchmark constants */

#define SEED 123								// Seed for rng
#define N 100									// Dimension of test matricies
#define NUM_ITERATIONS 1000000					// Number of iterations for square tests
#define MC_ITERATIONS 1000000					// Iterations for Monte Carlo test
#define SOR_ITERATIONS 1000						// Iterations for SOR test
#define OMEGA 1.25								// Parameter for SOR
#define REPS 10									// Number of times to repeat tests in "reps" functions
#define LUA_FILENAME "config.lua"
#define FFI_FILENAME "config_ffi.lua"
#define INSITU_FILENAME "config_insitu.lua"


/* Square tests - testing the speed of calling a function to square an int */

int CFunction(int val)
{
	return val * val;
}
int LuaJITFunction(lua_State *L, int val)
{
	lua_getglobal(L, "lua_square");
	lua_pushnumber(L, val);
	lua_pcall(L, 1, 1, 0);
	int retval = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return retval;
}
void CTest_sameval()
{
	clock_t t = clock();

	for (int i = 0; i < NUM_ITERATIONS; i++) {
		CFunction(100);
	}

	t = clock() - t;
	printf("C++ test with same argument repeated: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void CTest_incval() 
{
	clock_t t = clock();

	for (int i = 0; i < NUM_ITERATIONS; i++) {
		CFunction(i);
	}

	t = clock() - t;
	printf("C++ test with incrementing arguments: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LuaJITTest_sameval()
{	
	clock_t t = clock();

	lua_State *L = luaL_newstate();
	if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0)) {
		lua_error(L);
		return;
	}

	for (int i = 0; i < NUM_ITERATIONS; i++) {
		LuaJITFunction(L, 100);
	}

	t = clock() - t;
	printf("LuaJIT test with same argument repeated: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LuaJITTest_incval()
{	
	clock_t t = clock();

	lua_State *L = luaL_newstate();
	if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0)) {
		lua_error(L);
		return;
	}

	for (int i = 0; i < NUM_ITERATIONS; i++) {
		LuaJITFunction(L, i);
	}

	t = clock() - t;
	printf("LuaJIT test with incrementing arguments: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LuaJITTest_sameval_loop()
{	
	clock_t t = clock();

	lua_State *L = luaL_newstate();
	if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0)) {
		lua_error(L);
		return;
	}
	
	lua_getglobal(L, "lua_square_loop_same");
	lua_pushnumber(L, NUM_ITERATIONS);
	lua_pcall(L, 1, 0, 0);

	t = clock() - t;
	printf("LuaJIT test with repeated argument and computation done in Lua loop: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LuaJITTest_incval_loop()
{	
	clock_t t = clock();

	lua_State *L = luaL_newstate();
	if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0)) {
		lua_error(L);
		return;
	}
	
	lua_getglobal(L, "lua_square_loop_inc");
	lua_pushnumber(L, NUM_ITERATIONS);
	lua_pcall(L, 1, 0, 0);

	t = clock() - t;
	printf("LuaJIT test with incrementing arguments and computation done in Lua loop: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LejitTest_sameval()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int,int*)> stdf = [](int x, int *y){ return x; };
	lr->registerParam("lua_square", "i>i", stdf);
	lr->readParam("lua_square", stdf);

	int res = 0;
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		stdf(100, &res);
	}

	t = clock() - t;
	printf("LEJIT test with same repeated argument: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LejitTest_incval()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int,int*)> stdf = [](int x, int *y){ return x; };
	lr->registerParam("lua_square", "i>i", stdf);
	lr->readParam("lua_square", stdf);

	int res = 0;
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		stdf(i, &res);
	}

	t = clock() - t;
	printf("LEJIT test with incrementing arguments: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LejitTest_sameval_loop()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int)> stdf = [](int x){};
	lr->registerParam("lua_square_loop_same", stdf);
	lr->readParam("lua_square_loop_same", stdf);

	stdf(NUM_ITERATIONS);

	t = clock() - t;
	printf("LEJIT test with same repeated argument and computation done in Lua loop: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}
void LejitTest_incval_loop()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int)> stdf = [](int x){};
	lr->registerParam("lua_square_loop_inc", stdf);
	lr->readParam("lua_square_loop_inc", stdf);

	stdf(NUM_ITERATIONS);

	t = clock() - t;
	printf("LEJIT test with incrementing arguments and computation done in Lua loop: %f secs\n", ((float)t)/CLOCKS_PER_SEC);
}

/*
 * LU decomposition benchmark in C++
 */
double LU_C()
{
	clock_t t = clock();

	int pivot[N];
	srand(SEED);

	/* Generate a random matrix */
	double **A = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		A[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			A[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    for (int j = 0; j < N; j++)
    {

        int jp = j;
        
        double t = fabs(A[j][j]);
        for (int i = j + 1; i < N; i++)
        {
            double ab = fabs(A[i][j]);
            if (ab > t)
            {
                jp = i;
                t = ab;
            }
        }
        
        pivot[j] = jp;

        if ( A[jp][j] == 0 ) {
        	printf("Zero pivot\n");               
            return 0;
        }


        if (jp != j)
        {
            double *tA = A[j];
            A[j] = A[jp];
            A[jp] = tA;
        }

        if (j < N - 1) 
        {

            double recp =  1.0 / A[j][j];
            for (int k = j + 1; k < N; k++)
                A[k][j] *= recp;
        }


        if (j < N - 1)
        {

            for (int ii = j + 1; ii < N; ii++)
            {
                double *Aii = A[ii];
                double *Aj = A[j];
                double AiiJ = Aii[j];
                int jj;
                for (jj = j + 1; jj < N; jj++) 
                {
                    Aii[jj] -= AiiJ * Aj[jj];
                }

            }
        }
    }

    /* Return sum of diagonals */
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += A[i][i];
    }

	t = clock() - t;
	printf("C LU test: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * LU decomposition benchmark with no timing to be called by LuaJIT FFI
 */
extern "C" double LU()
{
	int pivot[N];
	srand(SEED);

	/* Generate a random matrix */
	double **A = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		A[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			A[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    for (int j = 0; j < N; j++)
    {

        int jp = j;
        
        double t = fabs(A[j][j]);
        for (int i = j + 1; i < N; i++)
        {
            double ab = fabs(A[i][j]);
            if (ab > t)
            {
                jp = i;
                t = ab;
            }
        }
        
        pivot[j] = jp;

        if ( A[jp][j] == 0 ) {
        	printf("Zero pivot\n");               
            return 0;
        }


        if (jp != j)
        {
            double *tA = A[j];
            A[j] = A[jp];
            A[jp] = tA;
        }

        if (j < N - 1)
        {
            double recp =  1.0 / A[j][j];
            for (int k = j + 1; k < N; k++)
                A[k][j] *= recp;
        }


        if (j < N - 1)
        {

            for (int ii = j + 1; ii < N; ii++)
            {
                double *Aii = A[ii];
                double *Aj = A[j];
                double AiiJ = Aii[j];
                int jj;
                for (jj = j + 1; jj < N; jj++) 
                {
                    Aii[jj] -= AiiJ * Aj[jj];
                }

            }
        }
    }

    /* Return sum of diagonals */
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += A[i][i];
    }

	return sum;
}

/*
 * LU decomposition benchmark performed in C called by LuaJIT FFI
 */
double LU_FFI()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(FFI_FILENAME);
	std::function<void(double*)> lua_lu_ffi;
	lr->registerParam("lua_lu_ffi", ">d", lua_lu_ffi);
	lr->readParam("lua_lu_ffi", lua_lu_ffi);

	double res = 0;
	lua_lu_ffi(&res);

	t = clock() - t;
	printf("Lejit LU test with computation done in C++ via LuaJIT FFI: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return res;
}

/*
 * LU decomposition benchmark with inner computation done in Lua
 */
double LU_Lejit()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double,double,double,double*)> lua_multsub;
	lr->registerParam("lua_multsub", "ddd>d", lua_multsub);
	lr->readParam("lua_multsub", lua_multsub);

	int pivot[N];
	srand(SEED);

	/* Generate a random matrix */
	double **A = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		A[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			A[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    for (int j = 0; j < N; j++)
    {

        int jp = j;
        
        double t = fabs(A[j][j]);
        for (int i = j + 1; i < N; i++)
        {
            double ab = fabs(A[i][j]);
            if (ab > t)
            {
                jp = i;
                t = ab;
            }
        }
        
        pivot[j] = jp;

        if ( A[jp][j] == 0 ) {
        	printf("Zero pivot\n");                
            return 0;
        }


        if (jp != j)
        {
            double *tA = A[j];
            A[j] = A[jp];
            A[jp] = tA;
        }

        if (j < N - 1)
        {
            double recp =  1.0 / A[j][j];
            for (int k = j + 1; k < N; k++)
                A[k][j] *= recp;
        }


        if (j < N - 1)
        {

            for (int ii = j + 1; ii < N; ii++)
            {
                double *Aii = A[ii];
                double *Aj = A[j];
                double AiiJ = Aii[j];
                int jj;
                for (jj = j + 1; jj < N; jj++) 
                {
                	lua_multsub(Aii[jj], AiiJ, Aj[jj], &Aii[jj]);
                }

            }
        }
    }

    /* Return sum of diagonals */
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += A[i][i];
    }

	t = clock() - t;
	printf("Lejit LU test with inner computation done in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * LU decomposition benchmark with computation done entirely in Lua
 */
double LU_Lejit_loop()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_lu;
	lr->registerParam("lua_lu", ">d", lua_lu);
	lr->readParam("lua_lu", lua_lu);

	double sum = 0;
	lua_lu(&sum);

	t = clock() - t;
	printf("Lejit LU test with computation done entirely in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * LU decomposition benchmark with computation done entirely in Lua
 * Repeated REPS times
 */
double LU_Lejit_loop_reps()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_lu;
	lr->registerParam("lua_lu", "d>", lua_lu);
	lr->readParam("lua_lu", lua_lu);

	double sum = 0;

	for (int r = 0; r < REPS; r++) {
		lua_lu(&sum);
	}

	t = clock() - t;
	printf("Lejit LU test with computation done entirely in Lua, repeated %d times: %f secs\n", REPS, ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/* 
 * MonteCarlo benchmark in C++
 */
double MonteCarlo_C()
{
	clock_t t = clock();

    srand(SEED);

    int under_curve = 0;
    int count;
    double x, y, res;

    for (count=0; count < MC_ITERATIONS; count++)
    {
        x = ((double) rand() / (RAND_MAX));
        y = ((double) rand() / (RAND_MAX));

        if ( x*x + y*y <= 1.0)
             under_curve ++;       
    }

    res = ((double) under_curve / MC_ITERATIONS) * 4.0;

    t = clock() - t;
	printf("C montecarlo test: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

    return res;
}

/*
 * MonteCarlo benchmark with no timing to be called from LuaJIT FFI
 */
extern "C" double MonteCarlo()
{
	srand(SEED);

	int under_curve = 0;
	int count;
	double x, y, res;

	for (count=0; count < MC_ITERATIONS; count++)
	{
	    x = ((double) rand() / (RAND_MAX));
	    y = ((double) rand() / (RAND_MAX));

	    if ( x*x + y*y <= 1.0)
	         under_curve ++;       
	}

	res = ((double) under_curve / MC_ITERATIONS) * 4.0;

	return res;
}

/*
 * MonteCarlo benchmark performed in C called by LuaJIT FFI
 */
double MonteCarlo_FFI()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(FFI_FILENAME);
	std::function<void(double*)> lua_montecarlo_ffi;
	lr->registerParam("lua_montecarlo_ffi", ">d", lua_montecarlo_ffi);
	lr->readParam("lua_montecarlo_ffi", lua_montecarlo_ffi);

	double res = 0;
	lua_montecarlo_ffi(&res);

	t = clock() - t;
	printf("Lejit montecarlo test with computation done in C++ via LuaJIT FFI: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return res;
}

/* 
 * MonteCarlo benchmark with inner computation done in Lua
 */
double MonteCarlo_Lejit()
{
	clock_t t = clock();

    LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int)> lua_srand;
	std::function<void(double*)> lua_rand;
	lr->registerParam("lua_srand", lua_srand);
	lr->registerParam("lua_rand", ">d", lua_rand);
	lr->readParam("lua_srand", lua_srand);
	lr->readParam("lua_rand", lua_rand);

    lua_srand(SEED);

    int under_curve = 0;
    int count;
    double x, y, res;

    for (count=0; count < MC_ITERATIONS; count++)
    {
        lua_rand(&x);
        lua_rand(&y);

        if ( x*x + y*y <= 1.0)
             under_curve ++;    
    }

    res = ((double) under_curve / MC_ITERATIONS) * 4.0;

    t = clock() - t;
	printf("LEJIT montecarlo test with inner computation done in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

    return res;
}

/* 
 * MonteCarlo benchmark with computation done entirely in Lua
 */
double MonteCarlo_Lejit_loop()
{
	clock_t t = clock();

    LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_montecarlo;
	lr->registerParam("lua_montecarlo", ">d", lua_montecarlo);
	lr->readParam("lua_montecarlo", lua_montecarlo);

	double res = 0;
	lua_montecarlo(&res);

    t = clock() - t;
	printf("LEJIT montecarlo test with computation done entirely in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

    return res;
}

/* 
 * MonteCarlo benchmark with computation done entirely in Lua
 * Repeated REPS times
 */
double MonteCarlo_Lejit_loop_reps()
{
	clock_t t = clock();

    LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_montecarlo;
	lr->registerParam("lua_montecarlo", ">d", lua_montecarlo);
	lr->readParam("lua_montecarlo", lua_montecarlo);

	double res = 0;

	for (int r = 0; r < REPS; r++) {
		lua_montecarlo(&res);
	}

    t = clock() - t;
	printf("LEJIT montecarlo test with computation done entirely in Lua, repeated %d times: %f secs\n", REPS, ((float)t)/CLOCKS_PER_SEC);

    return res;
}

/* 
 * MonteCarlo benchmark with computation done entirely in Lua
 * Includes insitu vis done in Lua
 */
double MonteCarlo_Lejit_loop_insitu()
{
	clock_t t = clock();

    LEJITReader *lr = new LEJITReader(INSITU_FILENAME);
	std::function<void(double*)> lua_montecarlo_insitu;
	lr->registerParam("lua_montecarlo_insitu", ">d", lua_montecarlo_insitu);
	lr->readParam("lua_montecarlo_insitu", lua_montecarlo_insitu);

	double res = 0;
	lua_montecarlo_insitu(&res);

    t = clock() - t;
	printf("LEJIT montecarlo test with computation done entirely in Lua and insitu: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

    return res;
}

/*
 * SOR benchmark in C++
 */
double SOR_C()
{
	clock_t t = clock();

	srand(SEED);

	/* Generate a random matrix */
	double **G = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		G[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			G[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    double omega_over_four = OMEGA * 0.25;
    double one_minus_omega = 1.0 - OMEGA;

    /* Update interior points */

    int Nm1 = N - 1; 
    double *Gi, *Gim1, *Gip1;

    for (int p = 0; p < SOR_ITERATIONS; p++)
    {
        for (int i = 1; i < Nm1; i++)
        {
            Gi = G[i];
            Gim1 = G[i-1];
            Gip1 = G[i+1];
            for (int j = 1; j < Nm1; j++) {
                Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] 
                            + Gi[j+1]) + one_minus_omega * Gi[j];
            }
        }
    }

    // Return sum of diagonals
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += G[i][i];
    }

    t = clock() - t;
	printf("C SOR test: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * SOR benchmark with no timing ot be called by LuaJIT FFI
 */
extern "C" double SOR()
{
	srand(SEED);

	/* Generate a random matrix */
	double **G = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		G[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			G[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    double omega_over_four = OMEGA * 0.25;
    double one_minus_omega = 1.0 - OMEGA;

    /* Update interior points */

    int Nm1 = N - 1; 
    double *Gi, *Gim1, *Gip1;

    for (int p = 0; p < SOR_ITERATIONS; p++)
    {
        for (int i = 1; i < Nm1; i++)
        {
            Gi = G[i];
            Gim1 = G[i-1];
            Gip1 = G[i+1];
            for (int j = 1; j < Nm1; j++) {
                Gi[j] = omega_over_four * (Gim1[j] + Gip1[j] + Gi[j-1] 
                            + Gi[j+1]) + one_minus_omega * Gi[j];
            }
        }
    }

    // Return sum of diagonals
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += G[i][i];
    }

	return sum;
}

/*
 * SOR benchmark performed in C called by LuaJIT FFI
 */
double SOR_FFI()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(FFI_FILENAME);
	std::function<void(double*)> lua_sor_ffi;
	lr->registerParam("lua_sor_ffi", ">d", lua_sor_ffi);
	lr->readParam("lua_sor_ffi", lua_sor_ffi);

	double res = 0;
	lua_sor_ffi(&res);

	t = clock() - t;
	printf("Lejit SOR test with computation done in C++ via LuaJIT FFI: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return res;
}

/*
 * SOR benchmark with inner computation done in Lua
 */
double SOR_Lejit()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(int,double,double,double*,double*,double*)> lua_sorinner;
	lr->registerParam("lua_sorinner", "iddad1ad1ad1", lua_sorinner);
	lr->readParam("lua_sorinner", lua_sorinner);

	srand(SEED);

	/* Generate a random matrix */
	double **G = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		G[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			G[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    double omega_over_four = OMEGA * 0.25;
    double one_minus_omega = 1.0 - OMEGA;

    /* Update interior points */

    int Nm1 = N - 1; 
    double *Gi, *Gim1, *Gip1;

    for (int p = 0; p < SOR_ITERATIONS; p++)
    {
        for (int i = 1; i < Nm1; i++)
        {
            Gi = G[i];
            Gim1 = G[i-1];
            Gip1 = G[i+1];
            for (int j = 1; j < Nm1; j++) {
            	lua_sorinner(j, omega_over_four, one_minus_omega, Gi, Gim1, Gip1);
            }
        }
    }

    // Return sum of diagonals
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += G[i][i];
    }

    t = clock() - t;
	printf("Lejit SOR test with inner computation done in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * SOR benchmark with 1 inner loop done in Lua
 */
double SOR_Lejit_loop_1()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double,double,double*,double*,double*)> lua_sor1loop;
	lr->registerParam("lua_sor1loop", "ddad1ad1ad1", lua_sor1loop);
	lr->readParam("lua_sor1loop", lua_sor1loop);

	srand(SEED);

	/* Generate a random matrix */
	double **G = (double **) malloc(sizeof(double*)*N);
	for (int i = 0; i < N; i++) {
		G[i] = (double *) malloc(sizeof(double)*N);
		for (int j = 0; j < N; j++) {
			G[i][j] = ((double) rand() / (RAND_MAX));
		}
	}

    double omega_over_four = OMEGA * 0.25;
    double one_minus_omega = 1.0 - OMEGA;

    /* Update interior points */

    int Nm1 = N - 1; 
    double *Gi, *Gim1, *Gip1;

    for (int p = 0; p < SOR_ITERATIONS; p++)
    {
    	for (int i = 1; i < Nm1; i++)
        {
            Gi = G[i];
            Gim1 = G[i-1];
            Gip1 = G[i+1];

    		lua_sor1loop(omega_over_four, one_minus_omega, Gi, Gim1, Gip1);
    	}
    }

    // Return sum of diagonals
    double sum = 0;
    for (int i = 0; i < N; i++) {
    	sum += G[i][i];
    }

	t = clock() - t;
	printf("Lejit SOR test with 1/3 loops in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}


/*
 * SOR benchmark with entire computation (3 nested loops) done in Lua
 */
double SOR_Lejit_loop_3()
{
	clock_t t = clock();

	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_sor;
	lr->registerParam("lua_sor", ">d", lua_sor);
	lr->readParam("lua_sor", lua_sor);

	double sum = 0;
	lua_sor(&sum);

	t = clock() - t;
	printf("Lejit SOR test with 3/3 loops in Lua: %f secs\n", ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * SOR benchmark with entire computation (3 nested loops) done in Lua
 * Repeated REPS times
 */
double SOR_Lejit_loop_3_reps()
{
	clock_t t = clock();
	LEJITReader *lr = new LEJITReader(LUA_FILENAME);
	std::function<void(double*)> lua_sor;
	lr->registerParam("lua_sor", ">d", lua_sor);
	lr->readParam("lua_sor", lua_sor);

	double sum = 0;

	for (int r = 0; r < REPS; r++) {
		lua_sor(&sum);
	}

	t = clock() - t;
	printf("Lejit SOR test with 3/3 loops in Lua, repeated %d times: %f secs\n", REPS, ((float)t)/CLOCKS_PER_SEC);

	return sum;
}

/*
 * main: calls all the test functions
 */
int main() 
{
	/* Square tests */
	//CTest_sameval();
	//CTest_incval();
	//LuaJITTest_sameval();
	//LuaJITTest_incval();
	//LuaJITTest_sameval_loop();
	//LuaJITTest_incval_loop();
	//LejitTest_sameval();
	//LejitTest_incval();
	//LejitTest_sameval_loop();
	//LejitTest_incval_loop();

	printf("----- LU DECOMPOSITION (%d x %d) -----\n", N, N);
	LU_C();
	LU_FFI();
	LU_Lejit();
	LU_Lejit_loop();
	LU_Lejit_loop_reps();
	printf("\n");
	
	printf("----- MONTE CARLO PI CALC (%d samples) -----\n", NUM_ITERATIONS);
	MonteCarlo_C();
	MonteCarlo_FFI();
	MonteCarlo_Lejit();
	MonteCarlo_Lejit_loop();
	MonteCarlo_Lejit_loop_reps();
	MonteCarlo_Lejit_loop_insitu();
	printf("\n");

	printf("----- SOR (%d x %d) -----\n", N, N);
	SOR_C();
	SOR_FFI();
	SOR_Lejit();
	SOR_Lejit_loop_1();
	SOR_Lejit_loop_3();
	SOR_Lejit_loop_3_reps();
	printf("\n");
	
	return 0;
}
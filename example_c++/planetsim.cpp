/*
 * planetsim.cpp   Dylan Everingham 01/25/2016
 *
 * Simple model of the trajectory of one solar system planet as a 2-body 
 * problem using the leapfrog method
 *
 * Usage: ./planetsim
 * 		configurable parameters can be set in the generated lua 
 * 		configuration file
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Include LEJIT library
#include "../LEJIT/Lejit.hpp"

#define MIN_PLOT_PTS 80				/* Min number of points to plot for 
									 * a trajectory */

#define BLOCKS 0					/* True if you want output data to be separated
					   	 			 * with newlines */

#define PRINT_ONCE 1				/* True if you want to only print the first
									 * data point */

#define DIMS 3						/* Do calculations in 3D. Don't change. */

#define G 6.67408E-11				/* Gravitational constant in m^3 kg^-1 s^-2 */
#define M 1.98855E30				/* Mass of the sun in kg */
#define MtoAU 6.68459E-12			/* Unit conversion factor */
#define DaytoSec 86400

/*
 * init_acc: initializes planet acceleration
 */
void init_acc(double r[DIMS], double a[DIMS]) {
	double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
	for (int k = 0; k < DIMS; k++) {
		a[k] = (-r[k] * G * M) / (r2 * sqrt(r2));
	}
}

/* 
 * leapstep: performs one timestep of planet movement
 */
void leapstep(double r[DIMS], double v[DIMS], double a[DIMS], int dt) {
	/* First, update pos and vel based on previous accel */
	for (int k = 0; k < DIMS; k++) {
		v[k] += 0.5 * a[k] * dt;
		r[k] += v[k] * dt;
	}

	/* Second, update accel and vel based on gravity interaction */
	double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
	for (int k = 0; k < DIMS; k++) {
		a[k] = (-r[k] * G * M) / (r2 * sqrt(r2));
		v[k] += 0.5 * a[k] * dt;
	}
}

/*
 * print_state: prints formatted system variable output
 * 
 * Works only for DIMS = 3
 */
void print_state(double r[], double v[], double a[], double t) 
{
	double C = MtoAU;		/* Conversion factor */
	
	printf("%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",
		t, r[0]*C, r[1]*C, r[2]*C, v[0]*C, v[1]*C, v[2]*C, a[0]*C, a[1]*C, a[2]*C);

	if (BLOCKS) { printf("\n\n"); }
}

/*
 * main: sets up system variables, initializes them and then calls leapstep
 * 		in loop
 */
int main(int argc, char *argv[])
{
	/* SOLAR SYSTEM DATA
 	 * format: x,y,z,vx,vy,vz,rotational period
 	 */
	double Mercury[7] = {-5.32961E10, 1.20481E10, 5.88222E9, -2.05354E4, -4.54805E4, -1.83316E3, 88.0};
	double Venus[7] = {-7.81584E10, -7.39467E10, 3.50136E9, 2.37726E4, -2.56407E4, -1.72352E3, 225.0};
	double Earth[7] = {-8.22372E10, 1.22016E11, -2.82248E7, -2.51183E4, -1.68486E4, 6.87167E-1, 365.0};
	double Mars[7] = {-2.44666E11, -2.02071E10, 5.56594E9, 2.91659E3, -2.20642E4, -5.34207E2, 687.0};
	double Jupiter[7] = {-7.81630E11, 2.13354E11, 1.65933E10, -3.59496E3, -1.19884E4, 1.30192E2, 4333.0};
	double Saturn[7] = {-5.37552E11, -1.39707E12, 4.56853E10, 8.48725E3, -3.49725E3, -2.77381E2, 10759.0};
	double Uranus[7] = {2.81845E12, 9.93259E11, -3.28247E10, -2.31296E3, 6.10534E3, 5.27699E2, 30688.0};
	double Neptune[7] = {4.18591E12, -1.60148E12, -6.34891E10, 1.90625E3, 5.10944E3, -1.48629E2, 60182.0};
	double Pluto[7] = {1.28796E12, -4.76811E12, 1.37663E11, 5.35329E3, 3.07364E2, -1.55957E3, 90581.0};

	double* Planets[9] = {Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto};
	
	double r[DIMS], v[DIMS], a[DIMS];

	/* DEOMONSTRATION OF USAGE OF LEJITREADER */

	// Declare some example configurable parameters with default values
	int planetnum = 4;
	int numrevs = 10;
	int nout = 5;
	std::string teststring = "Hello World!";
	std::function<void(int, int, int*)> do_calc = [](int x, int y, int *result){};
	std::function<void(double*)> arr_func = [](double *arr){};
	int testarr[3] = {4,2,42};
	int testarr2d[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
	double testarr3d[2][2][2] = { { {1.11, 1.12}, {1.21, 1.22} }, { {2.11, 2.12}, {2.21, 2.22} } };
	std::vector<int> testvec = {1, 2, 3};

	// Create new config file with default name "config.lua"
	LEJITReader *lr = new LEJITReader();

	// Register each configurable parameter with LEJIT
	lr->registerParam("planetnum", planetnum, 
		"Planetnum: number of planet to simulate i.e. 1 = Mercury, 2 = Venus, etc.");

	lr->registerParam("numrevs", numrevs,
		"Numrevs: the number of revolutions around the Sun to simulate");

	lr->registerParam("nout", nout,
		"Nout: The number of steps performed between each output data point.");

	lr->registerParam("teststring", teststring);

	lr->registerParam("do_calc", "ii>i", do_calc, "A test callback std::function");

	lr->registerParam("arr_func", arr_func, "A test std::function which takes an array argument");

	lr->registerParam("testarr", testarr);

	lr->registerParam("testarr2d", testarr2d);

	lr->registerParam("testarr3d", testarr3d);

	lr->registerParam("testvec", testvec);

	// Write config file. If it already exists, instead read parameter values out of it
	if(!lr->writeConfigFile()) {
		lr->readParam("planetnum", planetnum);
		lr->readParam("numrevs", numrevs);
		lr->readParam("nout", nout);
		lr->readParam("teststring", teststring);
		lr->readParam("do_calc", do_calc);
		lr->readParam("arr_func", arr_func);
		lr->readParam("testarr", testarr);
		lr->readParam("testarr2d", testarr2d);
		lr->readParam("testarr3d", testarr3d);
		lr->readParam("testvec", testvec);
	}

	// Test that parameters have been read correctly
	printf("planetnum is: %i\n", planetnum);
	printf("numrevs is: %i\n", numrevs);
	printf("nout is: %i\n", nout);
	printf("teststring is : %s\n", teststring.c_str());

	int x = 2, y = 3;
	int res = 0;
	printf("calling do_calc(%d, %d)...\n", x, y);
	do_calc(x, y, &res);
	printf("The result is %d\n", res);

	printf("calling arr_func...\n");
	double darr[3] = {1.1, 3.3, 5.5};
	arr_func(darr);

	printf("testarr = {%d, %d, %d}\n", testarr[0], testarr[1], testarr[2]);

	printf("testarr2d = {{%d, %d, %d},{%d, %d, %d},{%d, %d, %d}}\n", 
		testarr2d[0][0], testarr2d[0][1], testarr2d[0][2], 
		testarr2d[1][0], testarr2d[1][1], testarr2d[1][2], 
		testarr2d[2][0], testarr2d[2][1], testarr2d[2][2]);

	printf("testarr3d = {{{%f, %f}, {%f, %f}},{{%f, %f}, {%f, %f}}}\n", 
		testarr3d[0][0][0], testarr3d[0][0][1], testarr3d[0][1][0], testarr3d[0][1][1],
		testarr3d[1][0][0], testarr3d[1][0][1], testarr3d[1][1][0], testarr3d[1][1][1]);

	/* END LEJITREADER SECTION */


	/* MODEL CODE */

	// Choose planet and initialize position and velocity
	double *Planet = Planets[planetnum];
	r[0] = Planet[0];
	r[1] = Planet[1];
	r[2] = Planet[2];
	v[0] = Planet[3];
	v[1] = Planet[4];
	v[2] = Planet[5];

	int maxstep = ((int) Planet[6]) * numrevs;

	while (maxstep/nout < MIN_PLOT_PTS) { nout--; } 

	// Size of timestep
	double dt = DaytoSec;

	// Keeps track of current time
	double tnow = 0;		

	// Initialize acceleration
	init_acc(r, a);

	for (int nstep = 0; nstep < maxstep; nstep++) {
		
		// Print out data point
		if (PRINT_ONCE) {
			if (nstep == 0) {
				print_state(r, v, a, tnow);
			}
		}
		else if (nstep % nout == 0) {
			print_state(r, v, a, tnow);
		}

		// Perform an integration step
		leapstep(r, v, a, dt);
		
		// Update current time
		tnow += dt;
	}
}
#include <stdio.h>

// Include LEJIT library
#include "../LEJIT/Lejit.h"

int main(int argc, char *argv[])
{
	// Create reader
	LEJITReader_C *reader = LEJITReader_Create("test.lua");

	// Register params
	LEJITReader_RegisterInt(reader, "testint", 42);
	LEJITReader_RegisterFunction_ii(reader, "testfunc");
	LEJITReader_RegisterFunction_adad(reader, "testarrfunc");

	// Write config file
	LEJITReader_WriteConfigFile(reader);

	// Read params
	int i = 0;
	LEJITReader_ReadInt(reader, "testint", &i);
	printf("The value of i = %d\n", i);

	int x = 5, y = 4;
	LEJITReader_ReadFunction_ii(reader, "testfunc");
	LEJITReader_CallFunction_ii(reader, "testfunc", x, y);

	double arr0[3] = {1.0, 2.0, 3.0};
	double arr1[3] = {4.0, 5.0, 6.0};
	LEJITReader_ReadFunction_adad(reader, "testarrfunc");
	LEJITReader_CallFunction_adad(reader, "testarrfunc", arr0, arr1);
	printf("arr0[0] = %f\n", arr0[0]);
}
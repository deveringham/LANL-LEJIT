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
 * Lejit.h
 *
 * C interface for LEJIT
 * Uses the LEJITReader_C struct class in place of the C++ LEJITReader class
 *
 */

/*
 * Currently supported function callback signatures (see Lejit.hxx for key):
 * i
 * ii
 * d
 * >d
 * dd
 * dd>d
 * ddd>d
 * ai
 * ad
 * aiai
 * adad
 */

#ifndef LEJIT_H
#define LEJIT_H

struct LEJITReader_C;
typedef struct LEJITReader_C LEJITReader_C;

LEJITReader_C * LEJITReader_Create(const char *filename);

// Register Params
void LEJITReader_RegisterInt(LEJITReader_C *reader, const char *id, int def_val);
void LEJITReader_RegisterDouble(LEJITReader_C *reader, const char *id, double def_val);
void LEJITReader_RegisterString(LEJITReader_C *reader, const char *id, char *def_val);
void LEJITReader_RegisterArray_Int1D(LEJITReader_C *reader, const char *id, int def_val[], int length);
void LEJITReader_RegisterArray_Double1D(LEJITReader_C *reader, const char *id, double def_val[], int length);
void LEJITReader_RegisterFunction_i(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_ii_i(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_ii(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_d(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction__d(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_dd(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_dd_d(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_ddd_d(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_ai(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_ad(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_aiai(LEJITReader_C *reader, const char *id);
void LEJITReader_RegisterFunction_adad(LEJITReader_C *reader, const char *id);

// Read Params
void LEJITReader_ReadInt(LEJITReader_C *reader, const char *id, int *ptr);
void LEJITReader_ReadDouble(LEJITReader_C *reader, const char *id, double *ptr);
void LEJITReader_ReadString(LEJITReader_C *reader, const char *id, char *ptr);
void LEJITReader_ReadArray_Int1D(LEJITReader_C *reader, const char *id, int *ptr, int length);
void LEJITReader_ReadArray_Double1D(LEJITReader_C *reader, const char *id, double *ptr, int length);

// Read function callbacks - reads function definition out of Lua config file and JIT compiles it.
// Call the corresponding CallLuaFunction in order to execute it
void LEJITReader_ReadFunction_i(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_ii_i(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_ii(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_d(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction__d(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_dd(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_dd_d(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_ddd_d(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_ai(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_ad(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_aiai(LEJITReader_C *reader, const char *id);
void LEJITReader_ReadFunction_adad(LEJITReader_C *reader, const char *id);

// Call function callbacks - calls the most recently JITted version of the callback
void LEJITReader_CallFunction_i(LEJITReader_C *reader, const char *id, int arg0);
void LEJITReader_CallFunction_ii(LEJITReader_C *reader, const char *id, int arg0, int arg1);
void LEJITReader_CallFunction_ii_i(LEJITReader_C *reader, const char *id, int arg0, int arg1, int *arg2);
void LEJITReader_CallFunction_d(LEJITReader_C *reader, const char *id, double arg0);
void LEJITReader_CallFunction__d(LEJITReader_C *reader, const char *id, double *arg0);
void LEJITReader_CallFunction_dd(LEJITReader_C *reader, const char *id, double arg0, double arg1);
void LEJITReader_CallFunction_dd_d(LEJITReader_C *reader, const char *id, double arg0, double arg1, double *arg2);
void LEJITReader_CallFunction_ddd_d(LEJITReader_C *reader, const char *id, double arg0, double arg1, double arg2, double *arg3);
void LEJITReader_CallFunction_ai(LEJITReader_C *reader, const char *id, int arg0[]);
void LEJITReader_CallFunction_ad(LEJITReader_C *reader, const char *id, double arg0[]);
void LEJITReader_CallFunction_aiai(LEJITReader_C *reader, const char *id, int arg0[], int arg1[]);
void LEJITReader_CallFunction_adad(LEJITReader_C *reader, const char *id, double arg0[], double arg1[]);

// Write config file
int LEJITReader_WriteConfigFile(LEJITReader_C *reader);

#endif
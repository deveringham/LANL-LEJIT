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
 * Lejit.hpp
 *
 * C++ interface for LEGIT. Provides LEJITReader class
 *
 */

/*
 * Currently supported configurable parameter types:
 *	int
 *	double
 *  bool
 *	std::string
 *	C-style 1D, 2D or 3D array of int, double, bool or std::string
 *	C++ 1D, 2D, or 3D vector of int, double, boolean or std::string
 *	C++ std::function of any signature containing int, double, 
 *		boolean or 1D array of those types
 */

/*
 * Convention for signature strings:
 * i = int
 * d = double
 * b = bool
 * s = string (either std::string or char *)
 * a* = 1D array of non-array type * with no length specified (no bounds checking)
 * > = marks end of in parameters (arguments) and beginning of out parameters (results)
 *
 * All function parameters must have a void return values and return by reference (out parameter)
 */

#ifndef LEJIT_HPP
#define LEJIT_HPP

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <functional>
#include <algorithm>
#include <regex>

#include "Param.hpp"

// Default filename for Lua config file
#define DEFAULT_FILENAME "config.lua"

/*
 * Class LEJITReader
 *		Used keep track of registered configurable parameters, write the Lua
 *		config file and read / JIT compile configurable parameters from the
 *		config file.
 */
class LEJITReader {
private:
	// Name of Lua config file to be written / read from
	std::string filename;

	// Lua execution state
	lua_State *L;

	// List of all registered configurable parameters regardless of type
	std::map<std::string,Param*> paramlist;

	// Bools keeping track if torch and Gnuplot should be required in the config file
	bool torch_enabled, gnuplot_enabled;

	// Helper function to check if a string is an allowed Lua identifier
	bool isLuaIdentifier(std::string id);

	// Helper function to check if a parameter has already been registerd with
	// the same name but a different type
	Param * isRegistered(std::string id, std::type_index type);

	// Same as isRegistered but throws an error if not found
	Param * checkRegistered(std::string id, std::type_index type);

	// Helper functions for converting C-style arrays to C++ vectors
	template<typename T, size_t size> std::vector<T> convert1DArray(T (&arr)[size]);
	template<typename T, size_t rows, size_t cols> std::vector<std::vector<T>> convert2DArray(T (&arr)[rows][cols]);
	template<typename T, size_t rows, size_t cols, size_t depth> std::vector<std::vector<std::vector<T>>> convert3DArray(T (&arr)[rows][cols][depth]);


public:
	// Constructors
	LEJITReader() : LEJITReader(DEFAULT_FILENAME) {};
	LEJITReader(std::string filename, bool torch_enabled = 0, bool gnuplot_enabled = 0);

	// Template version for all non-array, non-function supported types
	template<typename T> void registerParam(std::string id, T def_val);
	template<typename T> void registerParam(std::string id, T def_val, std::string doc);

	// Specialization for C++ std::functions
	template<typename ...args> void registerParam(std::string id, std::function<void(args...)> def_val);
	template<typename ...args> void registerParam(std::string id, std::function<void(args...)> def_val, std::string doc);
	template<typename ...args> void registerParam(std::string id, std::string signature, std::function<void(args...)> def_val);
	template<typename ...args> void registerParam(std::string id, std::string signature, std::function<void(args...)> def_val, std::string doc);

	// Specialization for 1D C-style array types
	template<typename T, size_t size> void registerParam(std::string id, T (&def_val)[size]);
	template<typename T, size_t size> void registerParam(std::string id, T (&def_val)[size], std::string doc);

	// Specialization for 1D C++ dynamic array (vector) types
	template<typename T> void registerParam(std::string id, std::vector<T> def_val);
	template<typename T> void registerParam(std::string id, std::vector<T> def_val, std::string doc);

	// Specialization for 2D C-style array types
	template<typename T, size_t rows, size_t cols> void registerParam(std::string id, T (&def_val)[rows][cols]);
	template<typename T, size_t rows, size_t cols> void registerParam(std::string id, T (&def_val)[rows][cols], std::string doc);

	// Specialization for 2D C++ vector types
	template<typename T> void registerParam(std::string id, std::vector<std::vector<T>> def_val);
	template<typename T> void registerParam(std::string id, std::vector<std::vector<T>> def_val, std::string doc);

	// Specialization for 3D C-style array types
	template<typename T, size_t rows, size_t cols, size_t depth> void registerParam(std::string id, T (&def_val)[rows][cols][depth]);
	template<typename T, size_t rows, size_t cols, size_t depth> void registerParam(std::string id, T (&def_val)[rows][cols][depth], std::string doc);

	// Specialization for 3D C++ vector types
	template<typename T> void registerParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val);
	template<typename T> void registerParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val, std::string doc);

	// Template version for all non-array, non-function supported types
	template<typename T> void readParam(std::string id, T &ptr);

	// Specialization for C++ std::functions
	template<typename ...args> void readParam(std::string id, std::function<void(args...)> &ptr);

	// Specializations for reading 1D array into a pointer, C-style array or C++ vector
	template<typename T> void readParam(std::string id, T *(&ptr), size_t size);
	template<typename T, size_t size> void readParam(std::string id, T (&ptr)[size]);
	template<typename T> void readParam(std::string id, std::vector<T> (&ptr));

	// Template versions for reading 2D array into a pointer, C-style array or C++ vector
	template<typename T, size_t arrdim> void readParam(std::string id, T *(&ptr)[arrdim], size_t rows);
	template<typename T, size_t rows, size_t cols> void readParam(std::string id, T (&ptr)[rows][cols]);
	template<typename T> void readParam(std::string id, std::vector<std::vector<T>> (&ptr));

	// Template versions for reading 3D array into a pointer, C-style array or C++ vector
	template<typename T, size_t arrdim0, size_t arrdim1> void readParam(std::string id, T *(&ptr)[arrdim0][arrdim1], size_t rows);
	template<typename T, size_t rows, size_t cols, size_t depth> void readParam(std::string id, T (&ptr)[rows][cols][depth]);
	template<typename T> void readParam(std::string id, std::vector<std::vector<std::vector<T>>> (&ptr));
	
	// Gets the most recently Jitted version of a function parameter
	template <typename ...args> void getMostRecentFunc(std::string id, std::function<void(args...)> &ptr);

	// Writes config file if it does not exist, does nothing and returns false if it does
	bool writeConfigFile();
};

#include "Lejit.hxx"

#endif
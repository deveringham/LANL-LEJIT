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
 * LuaUtil.hpp
 *
 * Interface for some Lua helper functions
 *
 */

#ifndef LUAUTIL_H
#define LUAUTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "./torch/install/include/lua.hpp"

// Macro used to get type information about parameters
#define type(x) std::type_index(typeid(x))

// Overloaded version of lua_error to include custom message
void lua_error(lua_State *L, std::string fmt, ...);

// Helper functions to handle Lua to C value conversion in a
// polymorphic way. Only implemented for allowed types
void lua_gettopvalue(lua_State *L, int &ptr);
void lua_gettopvalue(lua_State *L, double &ptr);
void lua_gettopvalue(lua_State *L, bool &ptr);
void lua_gettopvalue(lua_State *L, std::string &ptr);

template <typename T> void lua_gettopvalue(lua_State *L, T *ptr, size_t size);				// C pointer
template <typename T, size_t size> void lua_gettopvalue(lua_State *L, T (&ptr)[size]); 		// C array
template <typename T> void lua_gettopvalue(lua_State *L, std::vector<T> (&ptr));			// C++ vector

template <typename T, size_t cols> void lua_gettopvalue(lua_State *L, T (&ptr)[cols], size_t rows);
template <typename T, size_t rows, size_t cols> void lua_gettopvalue(lua_State *L, T (&ptr)[rows][cols]);
template <typename T> void lua_gettopvalue(lua_State *L, std::vector<std::vector<T>> (&ptr));

template <typename T, size_t cols, size_t depth> void lua_gettopvalue(lua_State *L, T (&ptr)[cols][depth], size_t rows);
template <typename T, size_t rows, size_t cols, size_t depth> void lua_gettopvalue(lua_State *L, T (&ptr)[rows][cols][depth]);
template <typename T> void lua_gettopvalue(lua_State *L, std::vector<std::vector<std::vector<T>>> (&ptr));

// Functions for pushing values into lua tables on the stack
void lua_pushtableindex(lua_State *L, int index, int value);
void lua_pushtableindex(lua_State *L, int index, double value);
void lua_pushtableindex(lua_State *L, int index, bool value);
void lua_pushtableindex(lua_State *L, int index, std::string &value);

// Function for pushing an entire 1D C-style array onto the lua stack in the form
// of a lua table
// Copies individual elements
template <typename T> void lua_pushcarray_copy(lua_State *L, T *arr, size_t size);

// Function for pushing C array onto the Lua stack in the form of a userdata
// containing a pointer to the array in memory
template <typename T> void lua_pushcarray(lua_State *L, T *arr);

// Same as lua_pushcarray but with a C++ vector
template <typename T> void lua_pushvector(lua_State *L, std::vector<T> (&vec));

// Call to give a Lua state acess to the custom defined array type
int lua_openarray (lua_State* L);

// Calls an arbitrary Lua function
void lua_callfunc(lua_State *L, std::string func_name, std::string signature, ...);

#endif
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
 * LuaUtil.cpp
 *
 * Implementation of Lua helper functions
 *
 */

#include "LuaUtil.hpp"

/*
 * lua_error: handles Lua errors
 *		fmt: formatted string containing error message to print
 */
void lua_error(lua_State *L, std::string fmt, ...)
{
	// Pass string arguments to printf
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt.c_str(), argp);
	va_end(argp);

	// Close the Lua state
	lua_close(L);

	exit(EXIT_FAILURE);
}

/*
 * lua_gettopvalue: fetches the value on the top of the lua stack and pops it
 * from the stack
 */
void lua_gettopvalue(lua_State *L, int &ptr)
{
	ptr = luaL_checkint(L, -1);
	lua_pop(L, 1);
}

void lua_gettopvalue(lua_State *L, double &ptr)
{
	ptr = luaL_checknumber(L, -1);
	lua_pop(L, 1);
}

void lua_gettopvalue(lua_State *L, bool &ptr)
{
	ptr = luaL_checknumber(L, -1);
	lua_pop(L, 1);
}

void lua_gettopvalue(lua_State *L, std::string &ptr)
{
	ptr = std::string(luaL_checkstring(L, -1));
	lua_pop(L, 1);
}

template<typename T> void lua_gettopvalue(lua_State *L, T *ptr, size_t size)
{
	printf("In gettopvalue\n");
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < size; i++) {
		// Get indexed table value
		lua_pushinteger(L, i + 1); 
		lua_gettable(L, -2);
		T val;
		lua_gettopvalue(L, val);
		ptr[i] = val;
	}

	lua_pop(L, 1);
	printf("Returning from gettopvalue\n");
}

template<typename T, size_t size> void lua_gettopvalue(lua_State *L, T (&ptr)[size])
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < size; i++) {
		// Get indexed table value
		lua_pushinteger(L, i + 1); 
		lua_gettable(L, -2);
		T val;
		lua_gettopvalue(L, val);
		ptr[i] = val;
	}

	lua_pop(L, 1);
}

template<typename T> void lua_gettopvalue(lua_State *L, std::vector<T> (&ptr))
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	size_t size = ptr.size();
	for (int i = 0; i < size; i++) {
		// Get indexed table value
		lua_pushinteger(L, i + 1); 
		lua_gettable(L, -2);
		T val;
		lua_gettopvalue(L, val);
		ptr[i] = val;
	}

	lua_pop(L, 1);
}

template <typename T, size_t cols> void lua_gettopvalue(lua_State *L, T (&ptr)[cols], size_t rows)
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < rows; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));
	}

	lua_pop(L, 1);
}

template<typename T, size_t rows, size_t cols> void lua_gettopvalue(lua_State *L, T (&ptr)[rows][cols])
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < rows; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));
	}

	lua_pop(L, 1);
}

template<typename T> void lua_gettopvalue(lua_State *L, std::vector<std::vector<T>> (&ptr))
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	size_t size = ptr.size();
	for (int i = 0; i < size; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));	
	}

	lua_pop(L, 1);
}

template <typename T, size_t cols, size_t depth> void lua_gettopvalue(lua_State *L, T (&ptr)[cols][depth], size_t rows)
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < rows; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));
	}

	lua_pop(L, 1);
}

template<typename T, size_t rows, size_t cols, size_t depth> void lua_gettopvalue(lua_State *L, T (&ptr)[rows][cols][depth])
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	for (int i = 0; i < rows; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));
	}

	lua_pop(L, 1);
}

template<typename T> void lua_gettopvalue(lua_State *L, std::vector<std::vector<std::vector<T>>> (&ptr))
{
	// Check if stack value is a table
	if (!lua_istable(L, -1)) {
		lua_error(L, "expected parameter value to be a lua table\n");
	}

	size_t size = ptr.size();
	for (int i = 0; i < size; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);
		lua_gettopvalue(L, *(ptr + i));
	}

	lua_pop(L, 1);
}

void lua_pushtableindex(lua_State *L, int index, int value)
{
	// Assumes there is already a lua table on top of the stack
	lua_pushinteger(L, index);
	lua_pushinteger(L, value);
	lua_settable(L, -3);
}

void lua_pushtableindex(lua_State *L, int index, double value)
{
	// Assumes there is already a lua table on top of the stack
	lua_pushinteger(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

void lua_pushtableindex(lua_State *L, int index, bool value)
{
	// Assumes there is already a lua table on top of the stack
	lua_pushinteger(L, index);
	lua_pushboolean(L, value);
	lua_settable(L, -3);
}

void lua_pushtableindex(lua_State *L, int index, std::string value)
{
	// Assumes there is already a lua table on top of the stack
	lua_pushinteger(L, index);
	lua_pushstring(L, value.c_str());
	lua_settable(L, -3);
}

template <typename T> void lua_pushcarray_copy(lua_State *L, T *arr, size_t size)
{
	// Create a new lua table on the stack
	lua_newtable(L);

	// Push each value into it in order
	for (int i = 0; i < size; i++) {
		lua_pushtableindex(L, i+1, arr[i]);
	}
}

template <typename T> void lua_pushcarray(lua_State *L, T *arr)
{
	// Attempt to use torch tensors
	T** parray = (T**) lua_newuserdata(L, sizeof(T**));
   	*parray = arr;

   	if (type(T) == type(int)) {
   		luaL_getmetatable(L, "array_int");
   	}
   	else if (type(T) == type(double)) {
   		luaL_getmetatable(L, "array_double");
   	}
   	else if (type(T) == type(bool)) {
   		luaL_getmetatable(L, "array_bool");
   	}
   	else {
   		lua_error(L, "Type %s is not a supported array type", type(T).name());
   	}

   	lua_setmetatable(L, -2);
}

template <typename T> void lua_pushvector(lua_State *L, std::vector<T> (&vec))
{
	// Create a new lua table on the stack
	lua_newtable(L);

	// Push each value into it in order
	size_t size = vec.size();
	for (int i = 0; i < size; i++) {
		lua_pushtableindex(L, i+1, vec[i]);
	}
}

/* Functions for defining Lua array type using a userdata */

int array_index_int(lua_State* L) { 
   	int** parray = (int**) luaL_checkudata(L, 1, "array_int");
  	int index = luaL_checkint(L, 2);
   	lua_pushnumber(L, (*parray)[index-1]);
   	return 1; 
}

int array_newindex_int(lua_State* L) { 
   	int** parray = (int**) luaL_checkudata(L, 1, "array_int");
   	int index = luaL_checkint(L, 2);
   	int value = luaL_checkint(L, 3);
   	(*parray)[index-1] = value;
   	return 0; 
}

int array_index_double(lua_State* L) { 
   	double** parray = (double**) luaL_checkudata(L, 1, "array_double");
  	int index = luaL_checkint(L, 2);
   	lua_pushnumber(L, (*parray)[index-1]);
   	return 1; 
}

int array_newindex_double(lua_State* L) { 
   	double** parray = (double**) luaL_checkudata(L, 1, "array_double");
   	int index = luaL_checkint(L, 2);
   	double value = luaL_checknumber(L, 3);
   	(*parray)[index-1] = value;
   	return 0; 
}

int array_index_bool(lua_State* L) { 
   	bool** parray = (bool**) luaL_checkudata(L, 1, "array_bool");
  	int index = luaL_checkint(L, 2);
   	lua_pushboolean(L, (*parray)[index-1]);
   	return 1; 
}

int array_newindex_bool(lua_State* L) { 
   	bool** parray = (bool**) luaL_checkudata(L, 1, "array_bool");
   	int index = luaL_checkint(L, 2);
   	bool value = luaL_checknumber(L, 3);
   	(*parray)[index-1] = value;
   	return 0; 
}

void create_array_type_int(lua_State* L) {
   	static const struct luaL_reg array[] = {
      	{ "__index",  array_index_int  },
      	{ "__newindex",  array_newindex_int  },
      	{ NULL, NULL }
   	};
   	luaL_newmetatable(L, "array_int");
   	luaL_openlib(L, NULL, array, 0);
}

void create_array_type_double(lua_State* L) {
   	static const struct luaL_reg array[] = {
      	{ "__index",  array_index_double  },
      	{ "__newindex",  array_newindex_double  },
      	{ NULL, NULL }
   	};
   	luaL_newmetatable(L, "array_double");
   	luaL_openlib(L, NULL, array, 0);
}

void create_array_type_bool(lua_State* L) {
   	static const struct luaL_reg array[] = {
      	{ "__index",  array_index_bool  },
      	{ "__newindex",  array_newindex_bool  },
      	{ NULL, NULL }
   	};
   	luaL_newmetatable(L, "array_bool");
   	luaL_openlib(L, NULL, array, 0);
}

int lua_openarray(lua_State* L) {
   	create_array_type_int(L);
   	create_array_type_double(L);
   	create_array_type_bool(L);
   	return 0;
}

/*
 * call_lua: calls an arbitrary function written in Lua
 *
 *		func_name: name of function to be executed (must be present in L)
 *		signature: type specification for arguments and results. Of the form
 *			(type)*>(type)*
 *			where type can be: 
 *				i : int 
 *				d : double
 *				b : boolean
 *				ai : 1D C-style int array
 *				ad : 1D C-style double array
 *				ab : 1D C-style boolean array
 */
void lua_callfunc(lua_State *L, std::string func_name, std::string signature, ...)
{
	// Get argument list
	va_list v1;
	va_start(v1, signature);
	lua_getglobal(L, func_name.c_str());

	if (!lua_isfunction(L, -1)) {
		lua_error(L, "parameter value in config file should be a function\n");
	}

	const char *sig = signature.c_str();

	// Push arguments on to stack
	bool inparams_remaining = 1; 
	int num_inparams;
	for (num_inparams = 0; inparams_remaining; num_inparams++) {

		// If there are no inparams remaining, stop
		if(strlen(sig) == 0) {
			inparams_remaining = 0;
			break;
		}

		// Make sure there's enough stack space
		luaL_checkstack(L, 1, "not enough Lua stack space to hold arguments\n");

		// Swich on argument type
		switch (*sig++) {
			case 'i': {	// integer
				lua_pushinteger(L, va_arg(v1, int));
				break;
			}
			case 'd': {	// double
				lua_pushnumber(L, va_arg(v1, double));
				break;
			}
			case 'b': {	// boolean
				lua_pushboolean(L, va_arg(v1, int));
				break;
			}

			case 'a': {	// C-style array
				// use next charater to determine type of array
				switch (*sig++) {
					case 'i': {
						lua_pushcarray(L, va_arg(v1, int*));
						break;
					}
					case 'd': {
						lua_pushcarray(L, va_arg(v1, double*));
						break;
					}
					case 'b': {
						lua_pushcarray(L, va_arg(v1, bool*));
						break;
					}
					default:
						lua_error(L, "invalid argument type: (%c%c)\n", *(sig - 2), *(sig - 1));
				}
				break;
			}

			case '>': {	// end of inparams
				inparams_remaining = 0;
				num_inparams--;
				break;
			}
			default:
				lua_error(L, "invalid argument type: (%c)\n", *(sig - 1));
		}
	}

	// Get number of return values
	int num_outparams = strlen(sig);

	// Call the function
	if (lua_pcall(L, num_inparams, num_outparams, 0) != 0) {
		lua_error(L, "error calling function '%s': %s\n", func_name.c_str(), lua_tostring(L, -1));
	}

	// Start at bottom of stack
	num_outparams = -num_outparams;

	// While there are results remaining to output
	while(*sig) {
		// Switch on outparam type
		switch(*sig++) {
			case 'i': {	// integer
				int n = luaL_checkinteger(L, num_outparams);
				*va_arg(v1, int *) = n;
				break;
			}
			case 'd': {	// double
				double n = luaL_checknumber(L, num_outparams);
				*va_arg(v1, double *) = n;
				break;
			}
			case 'b': {	// boolean
				if (lua_isboolean(L, num_outparams)) {
					bool n = luaL_checkinteger(L, num_outparams);
					*va_arg(v1, bool *) = n;
				}
				else {
					lua_error(L, "return value is not boolean as expected\n");
				}
				break;
			}
			case 'a': {	// C-style array
				// use next charater to determine type of array
				switch (*sig++) {
					case 'i': {
						lua_gettopvalue(L, va_arg(v1, int*), 1);
						break;
					}
					case 'd': {
						lua_gettopvalue(L, va_arg(v1, double*), 1);
						break;
					}
					case 'b': {
						lua_gettopvalue(L, va_arg(v1, bool*), 1);
						break;
					}
					default:
						lua_error(L, "invalid argument type: (%c%c)\n", *(sig - 2), *(sig - 1));
				}
			}

			default:
				lua_error(L, "invalid result type: (%c)\n", *(sig - 1));
		}
		lua_pop(L, 1);
		num_outparams++;
	}

	va_end(v1);
}

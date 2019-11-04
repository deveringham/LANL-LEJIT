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
 * Param.cpp
 *
 * Implementation of Param and TypedParam classes used by LEGIT to store information 
 * about registered configurable parameters
 *
 */

#include "Param.hpp"

/*
 * default TypedParam constructor
 */
template <typename T> TypedParam<T>::TypedParam(std::string id, T def_val) 
{
	// Initialize member variables
	this->type = type(T);
	this->id = id;
	this->def_val = std::move(def_val);
	this->doc = "";
}

/*
 * TypedParam constructor with optional docstring
 */
template <typename T> TypedParam<T>::TypedParam(std::string id, T def_val, std::string doc)
{
	// Initialize member variables
	this->type = type(T);
	this->id = id;
	this->def_val = std::move(def_val);
	this->doc = doc;
}

/*
 * getLuaString: produces a string that can be printed to the Lua config file
 *		which represents as assignment to the parameter its default value.
 *		Also includes the optional docstring, if present.
 * 		This version is for all int double and bool parameters
 */
template <typename T> std::string TypedParam<T>::getLuaString()
{
	// Initialize return value
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = ";

	// Then add default value of the parameter
	ret += std::to_string(this->def_val);
	
	// Add newline after each parameter declaration
	ret += "\n";

	return ret;
}

/*
 * getLuaString: version for string parameters
 */
template <> std::string TypedParam<std::string>::getLuaString()
{
	// Initialize return value
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = ";

	// Then add default value of the parameter
	ret += "\"";
	ret += this->def_val;
	ret += "\"";
	
	// Add newline after each parameter declaration
	ret += "\n";

	return ret;
}

/*
 * TypedParam constructor for 1D c-style array parameters
 */
template <typename T> TypedParam<std::vector<T>>::TypedParam(std::string id, std::vector<T> def_val)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = "";

	// Use vector type
	this->type = type(std::vector<T>); 
}

/*
 * 1D array type TypedParam constructor with optional docstring
 */
template <typename T> TypedParam<std::vector<T>>::TypedParam(std::string id, std::vector<T> def_val, std::string doc)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = doc;

	// Use vector type
	this->type = type(std::vector<T>);
}

/*
 * getLuaString: version for 1d c-style array parameters
 */
template <typename T> std::string TypedParam<std::vector<T>>::getLuaString()
{

	// Initialize return value
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = ";

	// Brackets to enclose lua table
	ret += "{";

	// Then add default value of the parameter
	size_t size = this->getDims()[0];
	for(int i = 0; i < size-1; i++) {
		ret += std::to_string(this->def_val[i]);
		ret += ", ";
	}
	ret += std::to_string(this->def_val[size-1]);

	// Add close bracket and newline
	ret += "}\n";

	return ret;
}

/*
 * TypedParam constructor for 2D c-style array parameters
 */
template <typename T> TypedParam<std::vector<std::vector<T>>>::TypedParam(std::string id, std::vector<std::vector<T>> def_val)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = "";

	// Use vector type
	this->type = type(std::vector<std::vector<T>>); 
}

/*
 * 2D array type TypedParam constructor with optional docstring
 */
template <typename T> TypedParam<std::vector<std::vector<T>>>::TypedParam(std::string id, std::vector<std::vector<T>> def_val, std::string doc)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = doc;

	// Use vector type
	this->type = type(std::vector<std::vector<T>>);
}

/*
 * getLuaString: version for 2d c-style array parameters
 */
template <typename T> std::string TypedParam<std::vector<std::vector<T>>>::getLuaString()
{
	// Initialize return value
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = ";

	// Get table dimensions
	size_t rows = this->getDims()[0];
	size_t cols = this->getDims()[1];

	// Brackets to enclose Lua table
	ret += "{";

	for(int i = 0; i < rows; i++) {
		ret += "{";

		for (int j = 0; j < cols; j++) {
			ret += std::to_string(this->def_val[i][j]);
			if (j < cols-1) {
				ret += ", ";
			}
		}

		ret += "}";
		if (i < rows-1) {
			ret+= ", ";
		}
	}

	ret += "}\n";

	return ret;
}

/*
 * TypedParam constructor for 3D c-style array parameters
 */
template <typename T> TypedParam<std::vector<std::vector<std::vector<T>>>>::TypedParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = "";

	// Use vector type
	this->type = type(std::vector<std::vector<std::vector<T>>>); 
}

/*
 * 2D array type TypedParam constructor with optional docstring
 */
template <typename T> TypedParam<std::vector<std::vector<std::vector<T>>>>::TypedParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val, std::string doc)
{
	// Initialize member variables
	this->id = id;
	this->def_val = def_val;
	this->doc = doc;

	// Use vector type
	this->type = type(std::vector<std::vector<std::vector<T>>>);
}

/*
 * getLuaString: version for 3d c-style array parameters
 */
template <typename T> std::string TypedParam<std::vector<std::vector<std::vector<T>>>>::getLuaString()
{
	// Initialize return value
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = ";

	// Get table dimensions
	size_t rows = this->getDims()[0];
	size_t cols = this->getDims()[1];
	size_t depth = this->getDims()[2];

	// Brackets to enclose lua table
	ret += "{";

	for(int i = 0; i < rows; i++) {
		ret += "{";

		for (int j = 0; j < cols; j++) {
			ret += "{";

			for (int k = 0; k < depth; k++) {
				ret += std::to_string(this->def_val[i][j][k]);

				if (k < depth-1) {
					ret += ", ";
				}
			}

			ret += "}";

			if (j < cols-1) {
				ret += ", ";
			}
		}

		ret += "}";
		if (i < rows-1) {
			ret+= ", ";
		}
	}

	ret += "}\n";

	return ret;
}

/*
 * TypedParam constructor for C++ std::function type parameters
 */
template <typename ...args> TypedParam<std::function<void(args...)>>::TypedParam(lua_State *L, std::string id, std::string signature, std::function<void(args...)> def_val)
{
	// Initialize member variables
	this->L = L;
	this->type = type(std::function<void(args...)>);
	this->id = id;
	this->signature = signature;
	this->def_val = def_val;
	this->doc = "";
}

/*
 * C++ std::function type TypedParam constructor with optional docstring
 */
template <typename ...args> TypedParam<std::function<void(args...)>>::TypedParam(lua_State *L, std::string id, std::string signature, std::function<void(args...)> def_val, std::string doc)
{
	// Initialize member variables
	this->L = L;
	this->type = type(std::function<void(args...)>);
	this->id = id;
	this->signature = signature;
	this->def_val = def_val;
	this->doc = doc;
}

/*
 * getSignature: Returns a string describing the signature of the internal
 *		function. See Header comment on Lejit.hpp for description of signature
 *		strings.
 */
template <typename ...args> std::string TypedParam<std::function<void(args...)>>::getSignature()
{
	if (this->signature.length() > 0) {
		return this->signature;
	}
	else {
		// Construct new string
		std::string sig("");

		// For each argument...
		for (int argnum = 0; argnum < this->num_args; argnum++) {

			if (type(int) == this->argtypes[argnum]) {
				sig += 'i';
			}
			else if (type(double) == this->argtypes[argnum]) {
				sig += 'd';
			}
			else if (type(bool) == this->argtypes[argnum]) {
				sig += 'b';
			}

			// C-style array types
			else if (type(int*) == this->argtypes[argnum]) {
				sig += "ai";
			}
			else if (type(double*) == this->argtypes[argnum]) {
				sig += "ad";
			}
			else if (type(bool*) == this->argtypes[argnum]) {
				sig += "ab";
			}
		}

		this->signature = std::string(sig);
		return sig;
	}
}

/*
 * getLuaString: version for function parameters
 */
template <typename ...args> std::string TypedParam<std::function<void(args...)>>::getLuaString()
{
	std::string ret = "";

	// Start with docstring
	if (this->doc.length()) {
		ret += "--[[\n";
		ret += doc;
		ret += "\n--]]\n";
	}

	// Then do parameter identifier and =
	ret += this->id;
	ret += " = function(";

	const char * sig = this->getSignature().c_str();
	bool finished_inparams = 0;
	int i = 0;

	// Add an argument for each inparam in signature
	while(!finished_inparams && (*sig)) {
		switch(*sig++) {
			case '>' : 
				finished_inparams = 1;
				break;
			default:
				if (i > 0) {
					ret += ", ";
				}
				ret += "arg";
				ret += std::to_string(i);
				i++;
				break;
		}
	}
	ret += ")\n";

	// Add message indicating that this function can be redefined by the user
	ret += "\t-- Add a Lua function definition here to override the default behavior\n";
	ret += "end\n";

	return ret;
}

/*
 * getLuaFunc: returns std::function with Lua function read from config file bound to it,
 *		using lua_callfunc and std::bind
 */
template <typename ...args> std::function<void(args...)> TypedParam<std::function<void(args...)>>::getLuaFunc()
{
	this->most_recent = this->getLuaFunc(make_int_sequence<sizeof...(args)>{});
	return this->most_recent;
}
template <typename ...args> template<int ...Is> std::function<void(args...)> TypedParam<std::function<void(args...)>>::getLuaFunc(int_sequence<Is...>)
{
	return std::bind(&lua_callfunc, this->L, this->getId(), this->getSignature(), placeholder_template<Is>{}...);
}
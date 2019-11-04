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
 * Param.hpp
 *
 * Interface of Param and TypedParam classes used by LEGIT to store information 
 * about registered configurable parameters
 *
 */

#ifndef PARAM_H
#define PARAM_H

#include <vector>
#include <string>
#include <functional>
#include <typeindex>

#include "LuaUtil.cpp"

using namespace std::placeholders;

// Macro used to get type information about parameters
#define type(x) std::type_index(typeid(x))

/*
 * Param: wrapper class to allow TypedParams of any template specification
 *		to be stored in the same vector
 */
class Param {
protected:
	// Fields common to parameters of all types
	//enum ParamType type;
	std::type_index type = type(void);
	std::string id;
	std::string doc;

public:
	std::type_index getType() { return this->type; }
	std::string getId() { return this->id; }
	std::string getDoc() { return this->doc; }

	virtual std::string getLuaString() { return std::string("This type has no supported Lua representation!\n"); };

	// Treat all non array parameters as arrays of length 1
	virtual std::vector<size_t> getDims() { return std::vector<size_t> {1}; };

};

/*
 *  TypedParam: class to hold a parameter of a specific type
 */
template <typename T>
class TypedParam : public Param {
protected:
	// Default value of parameter
	T def_val;

public:
	// Constructors
	TypedParam(std::string id, T def_val);
	TypedParam(std::string id, T def_val, std::string doc);

	// Getter for default value
	T getDefVal() { return this->def_val; }

	// Produces string to be printed to Lua configuration file
	std::string getLuaString();
};

/* 
 * TypedParam specialization for 1d array types
 *		use a std::vector to represent the array
 */
template <typename T>
class TypedParam<std::vector<T>> : public Param {
private: 
	// Default value of parameter
	std::vector<T> def_val;

public:
	// Constructors
	TypedParam(std::string id, std::vector<T> def_val);
	TypedParam(std::string id, std::vector<T> def_val, std::string doc);

	// Getter for default value
	T getDefVal() { return this->def_val; }

	// Gets string expressing sunction signature and documentation which can be
	// printed to a Lua config file
	std::string getLuaString();

	// Getter for array size
	std::vector<size_t> getDims() { return std::vector<size_t> { this->def_val.size() }; };
};

/* 
 * TypedParam specialization for 2d array types
 *		use a std::vector to represent the array
 */
template <typename T>
class TypedParam<std::vector<std::vector<T>>> : public Param {
private: 
	// Default value of parameter
	std::vector<std::vector<T>> def_val;

public:
	// Constructors
	TypedParam(std::string id, std::vector<std::vector<T>> def_val);
	TypedParam(std::string id, std::vector<std::vector<T>> def_val, std::string doc);

	// Getter for default value
	T getDefVal() { return this->def_val; }

	// Gets string expressing sunction signature and documentation which can be
	// printed to a Lua config file
	std::string getLuaString();

	// Getter for array size
	std::vector<size_t> getDims() { return std::vector<size_t> { this->def_val.size(), this->def_val[0].size() }; };
};

/* 
 * TypedParam specialization for 3d array types
 *		use a std::vector to represent the array
 */
template <typename T>
class TypedParam<std::vector<std::vector<std::vector<T>>>> : public Param {
private: 
	// Default value of parameter
	std::vector<std::vector<std::vector<T>>> def_val;

public:
	// Constructors
	TypedParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val);
	TypedParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val, std::string doc);

	// Getter for default value
	T getDefVal() { return this->def_val; }

	// Gets string expressing sunction signature and documentation which can be
	// printed to a Lua config file
	std::string getLuaString();

	// Getter for array dimensions
	std::vector<size_t> getDims() { return std::vector<size_t> { this->def_val.size(), this->def_val[0].size(), this->def_val[0][0].size() }; };
};

/*
 * structs used for generating lists of std::placeholders for std::bind with 
 * variadic function templates
 */
template<int...> 
struct int_sequence {};

template<int N, int... Is> 
struct make_int_sequence
    : make_int_sequence<N-1, N-1, Is...> {};

template<int... Is> 
struct make_int_sequence<0, Is...>
    : int_sequence<Is...> {};

template<int> // begin with 0 here!
struct placeholder_template {};

namespace std
{
	template<int N>
	struct std::is_placeholder< placeholder_template<N> >
    	: std::integral_constant<int, N+1> {};
}


/*
 * TypedParam specialization for function types
 */
template <typename ...args>
class TypedParam<std::function<void(args...)>> : public Param {
private:
	// Default value of function
	std::function<void(args...)> def_val;

	// Most recently compiled version of the function
	std::function<void(args...)> most_recent = this->def_val;

	// Lua execution state the function definition will be found in
	lua_State *L;

	// String encoding signature of the interal function
	std::string signature;

	// Deduces signature string if one isnt provided from function type
	std::string getSignature();

	// List of arg types
	std::vector<std::type_index> argtypes { type(args)... };

	// Number of arguments
	static const size_t num_args = sizeof...(args);

	// Calls function defined in Lua
	//void call_lua(std::string func_name, ...);

	// Helper functions for identifying lengths of array arguments
	template<typename T, size_t size> std::vector<size_t> getArrayDims(T (&arr)[size]) { return std::vector<size_t> { size }; };
	template<typename T, size_t rows, size_t cols> std::vector<size_t> getArrayDims(T (&arr)[rows][cols]) { return std::vector<size_t> { rows, cols }; };
	template<typename T, size_t rows, size_t cols, size_t depth> std::vector<size_t> getArrayDims(T (&arr)[rows][cols][depth]) { return std::vector<size_t> { rows, cols, depth }; };

public:
	// Constructors
	TypedParam(lua_State *L, std::string id, std::function<void(args...)> def_val) : TypedParam(L, id, "", def_val) {}
	TypedParam(lua_State *L, std::string id, std::function<void(args...)> def_val, std::string doc) : TypedParam(L, id, "", def_val, doc) {}
	TypedParam(lua_State *L, std::string id, std::string signature, std::function<void(args...)> def_val);
	TypedParam(lua_State *L, std::string id, std::string signature, std::function<void(args...)> def_val, std::string doc);

	// Constructors that take and convert a function pointer (important note: no way to readParam() to a function pointer)
	TypedParam(lua_State *L, std::string id, void(*def_val)(args...)) : TypedParam(L, id, static_cast<std::function<void(args...)>>(def_val)) {}
	TypedParam(lua_State *L, std::string id, void(*def_val)(args...), std::string doc) : TypedParam(L, id, static_cast<std::function<void(args...)>>(def_val), doc) {}
	TypedParam(lua_State *L, std::string id, std::string signature, void(*def_val)(args...)) : TypedParam(L, id, signature, static_cast<std::function<void(args...)>>(def_val)) {}
	TypedParam(lua_State *L, std::string id, std::string signature, void(*def_val)(args...), std::string doc) : TypedParam(L, id, signature, static_cast<std::function<void(args...)>>(def_val), doc) {}

	// Getter for default value
	std::function<void(args...)> getDefVal() { return this->def_val; }

	// Getter for number of arguments taken by internal function
	size_t getNumArgs() { return this->num_args; }

	// Gets most recently compiled version of the function
	std::function<void(args...)> getMostRecent() { return this->most_recent; }

	// Gets string expressing sunction signature and documentation which can be
	// printed to a Lua config file
	std::string getLuaString();

	// Reads Lua function out of config file and binds it to a std::function, 
	// which it returns
	std::function<void(args...)> getLuaFunc();
	template<int ...Is> std::function<void(args...)> getLuaFunc(int_sequence<Is...>);
};

#endif
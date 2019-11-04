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
 * Lejit.hxx
 *
 * C++ implementation for LEJITReader Class
 * Extension is .hxx because Template class definitions must be in the header
 *
 */

#include "Param.cpp"

// Lua reserved words not allowed as parameter names
#define LUA_RESERVED_WORDS "and|break|do|else|elseif|end|false|for|function|goto|if|in|local|nil|not|or|repeat|return|then|true|until|while"

/* C wrappers */

extern "C"
{
	#include "Lejit.h"
	
	/*
	 * filename constructor
	 */
	LEJITReader_C * LEJITReader_Create(const char *filename)
	{
		return reinterpret_cast<LEJITReader_C*>( new LEJITReader(std::string(filename)) );
	}

	/*
	 * C wrapper of registerParam for ints
	 */
	void LEJITReader_RegisterInt(LEJITReader_C *reader, const char *id, int def_val)
	{
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), def_val);
	}

	/*
	 * C wrapper of registerParam for doubles
	 */
	void LEJITReader_RegisterDouble(LEJITReader_C *reader, const char *id, double def_val)
	{
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), def_val);
	}

	/*
	 * C wrapper of registerParam for strings
	 */
	void LEJITReader_RegisterString(LEJITReader_C *reader, const char *id, char *def_val)
	{
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), std::string(def_val));
	}

	/*
	 * C wrapper of registerParam for 1D int arrays
	 */
	void LEJITReader_RegisterArray_Int1D(LEJITReader_C *reader, const char *id, int def_val[], int length)
	{
		std::vector<int> vec(def_val, def_val + length);
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), vec);
	}

	/*
	 * C wrapper of registerParam for 1D double arrays
	 */
	void LEJITReader_RegisterArray_Double1D(LEJITReader_C *reader, const char *id, double def_val[], int length)
	{
		std::vector<double> vec(def_val, def_val + length);
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), vec);		
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ii"
	 */
	void LEJITReader_RegisterFunction_i(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ii"
	 */
	void LEJITReader_RegisterFunction_ii(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int,int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ii>i"
	 */
	void LEJITReader_RegisterFunction_ii_i(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int,int,int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), "ii>i", stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "d"
	 */
	void LEJITReader_RegisterFunction_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature ">d"
	 */
	void LEJITReader_RegisterFunction__d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), ">d", stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "dd"
	 */
	void LEJITReader_RegisterFunction_dd(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "dd>d"
	 */
	void LEJITReader_RegisterFunction_dd_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), "dd>d", stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ddd>d"
	 */
	void LEJITReader_RegisterFunction_ddd_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), "ddd>d", stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ai"
	 */
	void LEJITReader_RegisterFunction_ai(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "ad"
	 */
	void LEJITReader_RegisterFunction_ad(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "aiai"
	 */
	void LEJITReader_RegisterFunction_aiai(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int*,int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of registerParam for function callbacks with signature "adad"
	 */
	void LEJITReader_RegisterFunction_adad(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*,double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->registerParam(std::string(id), stdf);
	}

	/*
	 * C wrapper of readParam for ints
	 */
	void LEJITReader_ReadInt(LEJITReader_C *reader, const char *id, int *ptr)
	{
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), *ptr);
	}

	/*
	 * C wrapper of readParam for doubles
	 */
	void LEJITReader_ReadDouble(LEJITReader_C *reader, const char *id, double *ptr)
	{
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), *ptr);
	}

	/*
	 * C wrapper of readParam for strings
	 */
	void LEJITReader_ReadString(LEJITReader_C *reader, const char *id, char *ptr)
	{
		std::string s(ptr);
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), s);
		strcpy(ptr, s.c_str());
	}

	/*
	 * C wrapper of readParam for 1D int arrays
	 */
	void LEJITReader_ReadArray_Int1D(LEJITReader_C *reader, const char *id, int *ptr, int length)
	{
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), ptr, length);
	}

	/*
	 * C wrapper of readParam for 1D double arrays
	 */
	void LEJITReader_ReadArray_Double1D(LEJITReader_C *reader, const char *id, double *ptr, int length)
	{
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), ptr, length);
	}

	/*
	 * C wrapper used to read function parameter with signature "i" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_i(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "ii" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_ii(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int, int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "ii>i" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_ii_i(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int, int, int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "d" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature ">d" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction__d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "dd" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_dd(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "dd>d" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_dd_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "ddd>d" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_ddd_d(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double, double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "ai" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_ai(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "aiai" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_ad(LEJITReader_C *reader, const char *id)
	{
		std::function<void(int*, int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "ad" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_aiai(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to read function parameter with signature "adad" out of the config file
	 *	and JIT compile it. Does not call the function.
	 */
	void LEJITReader_ReadFunction_adad(LEJITReader_C *reader, const char *id)
	{
		std::function<void(double*, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->readParam(std::string(id), stdf);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "i"
	 */
	void LEJITReader_CallFunction_i(LEJITReader_C *reader, const char *id, int arg0)
	{
		std::function<void(int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "ii"
	 */
	void LEJITReader_CallFunction_ii(LEJITReader_C *reader, const char *id, int arg0, int arg1)
	{
		std::function<void(int, int)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "ii>i"
	 */
	void LEJITReader_CallFunction_ii_i(LEJITReader_C *reader, const char *id, int arg0, int arg1, int *arg2)
	{
		std::function<void(int, int, int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1, arg2);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "d"
	 */
	void LEJITReader_CallFunction_d(LEJITReader_C *reader, const char *id, double arg0)
	{
		std::function<void(double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature ">d"
	 */
	void LEJITReader_CallFunction__d(LEJITReader_C *reader, const char *id, double *arg0)
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "dd"
	 */
	void LEJITReader_CallFunction_dd(LEJITReader_C *reader, const char *id, double arg0, double arg1)
	{
		std::function<void(double, double)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "dd>d"
	 */
	void LEJITReader_CallFunction_dd_d(LEJITReader_C *reader, const char *id, double arg0, double arg1, double *arg2)
	{
		std::function<void(double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1, arg2);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "ddd>d"
	 */
	void LEJITReader_CallFunction_ddd_d(LEJITReader_C *reader, const char *id, double arg0, double arg1, double arg2, double *arg3)
	{
		std::function<void(double, double, double, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1, arg2, arg3);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "ai"
	 */
	void LEJITReader_CallFunction_ai(LEJITReader_C *reader, const char *id, int arg0[])
	{
		std::function<void(int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "ad"
	 */
	void LEJITReader_CallFunction_ad(LEJITReader_C *reader, const char *id, double arg0[])
	{
		std::function<void(double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "aiai"
	 */
	void LEJITReader_CallFunction_aiai(LEJITReader_C *reader, const char *id, int arg0[], int arg1[])
	{
		std::function<void(int*, int*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1);
	}

	/*
	 * C wrapper used to call most recently read version of function parameter with signature "adad"
	 */
	void LEJITReader_CallFunction_adad(LEJITReader_C *reader, const char *id, double arg0[], double arg1[])
	{
		std::function<void(double*, double*)> stdf;
		reinterpret_cast<LEJITReader*>(reader)->getMostRecentFunc(std::string(id), stdf);
		stdf(arg0, arg1);
	}

	/*
	 * C wrapper of writeConfigFile
	 */
	int LEJITReader_WriteConfigFile(LEJITReader_C *reader)
	{
		return reinterpret_cast<LEJITReader*>(reader)->writeConfigFile();
	}
}

/* C++ implementations */

/*
 * filename LEJITReader constructor
 */
LEJITReader::LEJITReader(std::string filename, bool torch_enabled, bool gnuplot_enabled)
{
	// Use given filename
	this->filename = filename;
	
	// Create new Lua state and open standard libraries
	this->L = luaL_newstate();
	luaL_openlibs(this->L);
	lua_openarray(this->L);

	// Set library enable flags
	this->torch_enabled = torch_enabled;
	this->gnuplot_enabled = gnuplot_enabled;
}

/*
 * isLuaIdentifier: returns true if id is a valid Lua identifier and throws an
 *		error if not
 */
bool LEJITReader::isLuaIdentifier(std::string id)
{
	std::regex rgx("[A-Za-z_][0-9A-Za-z_]*");
	if (std::regex_match(id, rgx)) {

		// Check if it is a reserved word
		std::regex reserved_words(LUA_RESERVED_WORDS);
		if (!std::regex_match(id, reserved_words)) {
			return true;
		}
	}

	throw std::invalid_argument(std::string("Parameter name '") + std::string(id.c_str())
		+ std::string("' is not a valid Lua identifier."));
}

/*
 * this->isRegistered: returns the registered Param if a parameter has been registered with type type.
 * 		otherwise returns null. If the parameter has been registered with a
 *		different type, throws an error.
 */
Param * LEJITReader::isRegistered(std::string id, std::type_index type)
{
	Param *p = nullptr;
	try {
		p = this->paramlist.at(id);
	}
	catch (std::out_of_range& e) {
		return p;
	}

	if (p->getType() == type) {
		return p;
	}
	else {
		throw std::invalid_argument(std::string("Parameter name '") + std::string(id.c_str())
			+ std::string("' is already registerd with a different type. Choose another identifier."));
	}
}

/*
 * this->checkRegistered: same as this->isRegistered but throws an error if the parameter is not found
 */
Param * LEJITReader::checkRegistered(std::string id, std::type_index type)
{
	Param *p = this->isRegistered(id, type);
	if (p) {
		return p;
	}
	else {
		// If param not found
		throw std::invalid_argument(std::string("Parameter '") + id
		+ std::string("' is not registered. Register parameter before calling readParam\n"));
	}
}

/*
 * convert1DArray: helper function to convert 1D c-style array types to std::vectors
 * 		arr: array to convert
 */
template<typename T, size_t size> std::vector<T> LEJITReader::convert1DArray(T (&arr)[size])
{
	// Create and populate new std::vector
	std::vector<T> v(arr, arr + size);
	return v;
}

/*
 * convert2DArray: helper function to convert 2D c-style array types to std::vectors
 * 		arr: array to convert
 */
template<typename T, size_t rows, size_t cols> std::vector<std::vector<T>> LEJITReader::convert2DArray(T (&arr)[rows][cols])
{
	// Create and populate new std::vector
	std::vector<std::vector<T>> v;
	for (int i = 0; i < rows; i++) {
		std::vector<T> vi = convert1DArray(*(arr + i));
		v.push_back(vi);
	}
	return v;
}

/*
 * convert3DArray: helper function to convert 3D c-style array types to std::vectors
 * 		arr: array to convert
 */
template<typename T, size_t rows, size_t cols, size_t depth> std::vector<std::vector<std::vector<T>>> LEJITReader::convert3DArray(T (&arr)[rows][cols][depth])
{
	// Create and populate new std::vector
	std::vector<std::vector<std::vector<T>>> v;
	for (int i = 0; i < rows; i++) {
		std::vector<std::vector<T>> vi = convert2DArray(*(arr + i));
		v.push_back(vi);
	}
	return v;
}

/*
 * registerParam: template version to handle all non-array, non-function
 *		supported types
 *		Throws an error if the parameter name is an invalid Lua identifier
 *		or if it has already been registered as a different type. If it has
 *		already been registered as the same type, does nothing.
 *
 *		id: name to give parameter in Lua config file
 *		def_val: default value to assign the parameter in the Lua config file
 *			if the parameter is a function, it is not assigned def_val in the
 *			Lua config file, but appears as an empty function declaration
 *			(may be changed in the future)
 */
template<typename T> void LEJITReader::registerParam(std::string id, T def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(T))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<T>(id, def_val));
		}
	}
}

/*
 * registerParam: template version with optional docstring
 *		doc: string which appears as a doc comment accompanying the parameter
 *			in the Lua config file
 */
template<typename T> void LEJITReader::registerParam(std::string id, T def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(T))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<T>(id, def_val, doc));
		}
	}
}

/*
 * registerParam: C++ std::function version
 */
template<typename ...args> void LEJITReader::registerParam(std::string id, std::function<void(args...)> def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::function<void(args...)>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::function<void(args...)>>(this->L, id, def_val));
		}
	}
}

/*
 * registerParam: C++ std::function version with optional docstring
 */
template<typename ...args> void LEJITReader::registerParam(std::string id, std::function<void(args...)> def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::function<void(args...)>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::function<void(args...)>>(this->L, id, def_val, doc));	
		}
	}
}

/*
 * registerParam: C++ std::function version with optional signature string
 */
template<typename ...args> void LEJITReader::registerParam(std::string id, std::string signature, std::function<void(args...)> def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::function<void(args...)>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::function<void(args...)>>(this->L, id, signature, def_val));
		}
	}
}

/*
 * registerParam: C++ std::function version with optional signature string and docstring
 */
template<typename ...args> void LEJITReader::registerParam(std::string id, std::string signature, std::function<void(args...)> def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::function<void(args...)>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::function<void(args...)>>(this->L, id, signature, def_val, doc));	
		}
	}
}


/*
 * registerParam: version for 1D C-style arrays
 *		size: length of the array
 */
template<typename T, size_t size> void LEJITReader::registerParam(std::string id, T (&def_val)[size])
{
 	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<T>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<T> v = convert1DArray(def_val);
			this->paramlist[id] = (new TypedParam<std::vector<T>>(id, v));
		}
	}
}

/*
 * registerParam: 1D C-style array type version with optional docstring
 */
template<typename T, size_t size> void LEJITReader::registerParam(std::string id, T (&def_val)[size], std::string doc) 
{
	 if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<T>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<T> v = convert1DArray(def_val, size);
			this->paramlist[id] = (new TypedParam<std::vector<T>>(id, v, doc));
		}
	}
}

/*
 * registerParam: version for 2D C-style arrays
 *		rows, cols: dimensions of the array
 */
template<typename T, size_t rows, size_t cols> void LEJITReader::registerParam(std::string id, T (&def_val)[rows][cols])
{
 	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<T>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<std::vector<T>> v = convert2DArray(def_val);
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<T>>>(id, v));
		}
	}
}

/*
 * registerParam: 2D C-style array type version with optional docstring
 */
template<typename T, size_t rows, size_t cols> void LEJITReader::registerParam(std::string id, T (&def_val)[rows][cols], std::string doc)
{
	 if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<T>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<std::vector<T>> v = convert2DArray(def_val);
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<T>>>(id, v, doc));
		}
	}
}

 /*
 * registerParam: version for 3D C-style arrays
 *		rows, cols, depth: dimensions of the array
 */
template<typename T, size_t rows, size_t cols, size_t depth> void LEJITReader::registerParam(std::string id, T (&def_val)[rows][cols][depth])
{
 	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<std::vector<std::vector<T>>> v = convert3DArray(def_val);
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<std::vector<T>>>>(id, v));
		}
	}
}

/*
 * registerParam: 3D C-style array type version with optional docstring
 */
template<typename T, size_t rows, size_t cols, size_t depth> void LEJITReader::registerParam(std::string id, T (&def_val)[rows][cols][depth], std::string doc)
{
	 if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			std::vector<std::vector<std::vector<T>>> v = convert3DArray(def_val);
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<std::vector<T>>>>(id, v, doc));
		}
	}
}

/*
 * registerParam: 1D C++ vector version
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<T> def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<T>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<T>>(id, def_val));
		}
	}
}

/*
 * registerParam: 1D C++ vector version with optional docstring
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<T> def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<T>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<T>>(id, def_val, doc));
		}
	}
}

/*
 * registerParam: 2D C++ vector version
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<std::vector<T>> def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<T>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<T>>>(id, def_val));
		}
	}
}

/*
 * registerParam: 2D C++ vector version with optional docstring
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<std::vector<T>> def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<T>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<T>>>(id, def_val, doc));
		}
	}
}

/*
 * registerParam: 3D C++ vector version
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<std::vector<T>>>>(id, def_val));
		}
	}
}

/*
 * registerParam: 3D C++ vector version with optional docstring
 */
template<typename T> void LEJITReader::registerParam(std::string id, std::vector<std::vector<std::vector<T>>> def_val, std::string doc)
{
	if (isLuaIdentifier(id)) {
		if (this->isRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
			return;
		}
		else {
			// If no match is found, create a new Param 
			this->paramlist[id] = (new TypedParam<std::vector<std::vector<std::vector<T>>>>(id, def_val, doc));
		}
	}
} 

/*
 * readParam: reads the named parameter out of the config file. Throws an
 *		error if the config file is unrunable of if the supplied value is
 *		absent or the wrong type. Templated to handle all non-function,
 *		non-array allowed types
 *
 *		id: identifier of parameter to fetch
 *		ptr: place to store fetched parameter value
 */
template<typename T> void LEJITReader::readParam(std::string id, T &ptr)
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(this->L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(T))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/*
 * readParam: C++ std::function type version
 */
template<typename ...args> void LEJITReader::readParam(std::string id, std::function<void(args...)> &ptr)
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	Param *p = this->checkRegistered(id, type(std::function<void(args...)>));
	ptr = ((TypedParam<std::function<void(args...)>>*)p)->getLuaFunc();
}

/* 
 * readParam: 1D array version, reading into a pointer
 */
template<typename T> void LEJITReader::readParam(std::string id, T *(&ptr), size_t size)
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<T>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr, size);
	}
}

/*
 * readParam: 1D array version, reading into a C-style array
 */ 
template<typename T, size_t size> void LEJITReader::readParam(std::string id, T (&ptr)[size])
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<T>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/*
 * readParam: 1D array version, reading into a C++ vector
 */
template<typename T> void LEJITReader::readParam(std::string id, std::vector<T> (&ptr))
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<T>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/* 
 * readParam: 2D c-style array version, reading into a pointer
 */
template<typename T, size_t arrdim> void LEJITReader::readParam(std::string id, T *(&ptr)[arrdim], size_t rows)
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<T>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr, rows);
	}
}

/*
 * readParam: 2D c-style array version, reading into a C-style array
 */
template<typename T, size_t rows, size_t cols> void LEJITReader::readParam(std::string id, T (&ptr)[rows][cols])
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<T>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/*
 * readParam: 2D array version, reading into a C++ vector
 */
template<typename T> void LEJITReader::readParam(std::string id, std::vector<std::vector<T>> (&ptr))
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<T>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/* 
 * readParam: 3D c-style array version, reading into a pointer
 */
template<typename T, size_t arrdim0, size_t arrdim1> void LEJITReader::readParam(std::string id, T *(&ptr)[arrdim0][arrdim1], size_t rows)
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr, rows);
	}
}

/*
 * readParam: 3D c-style array version, reading into an array
 */ 
template<typename T, size_t rows, size_t cols, size_t depth> void LEJITReader::readParam(std::string id, T (&ptr)[rows][cols][depth])
{

	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/*
 * readParam: 3D array version, reading into a C++ vector
 */
template<typename T> void LEJITReader::readParam(std::string id, std::vector<std::vector<std::vector<T>>> (&ptr))
{
	// Try to load the config file
	if (luaL_loadfile(this->L, this->filename.c_str()) || lua_pcall(this->L, 0, 0, 0)) {
		lua_error(L, "error in config file: %s\n", lua_tostring(this->L, -1));
	}

	if (this->checkRegistered(id, type(std::vector<std::vector<std::vector<T>>>))) {
		// Gets parameter value
		lua_getglobal(this->L, id.c_str());
		lua_gettopvalue(this->L, ptr);
	}
}

/*
 * getMostRecentFunc: Gets the most recently read version of a function parameter
 */
template<typename ...args> void LEJITReader::getMostRecentFunc(std::string id, std::function<void(args...)> &ptr)
{
	Param *p = this->checkRegistered(id, type(std::function<void(args...)>));
	ptr = ((TypedParam<std::function<void(args...)>>*)p)->getMostRecent();
}

/*
 * writeConfigFile: writes a Lua configuration file with all stored parameters
 *		as global variables set to their default values, which can be modified 
 * 		by a user and read with getParam. If file already exists, does not 
 * 		modify it and returns 0.
 */
bool LEJITReader::writeConfigFile()
{
	// Check if file exists
	std::ifstream check(this->filename);
	if (check) {
		return 0;
	}

	// Open file
	std::ofstream file(this->filename);

	if (file.is_open()) {
		// Write header
		file << "--[[\n";
		file << "\tThis is a Lua configuaration file generated with LEJITReader\n";
		file << "\tEdit any parameter here and rerrun the calling code without recompilation to\n";
		file << "\t\tsee altered behavior.\n";
		file << "--]]\n\n";

		if (this->torch_enabled) {
			file << "-- Include LuaJIT FFI Library\n";
			file << "local ffi = require 'ffi'\n\n";
		}

		if (this->gnuplot_enabled) {
			file << "-- Include torch libraries\n";
			file << "local th = require 'torch'\n\n";
		}
		
		// Write each parameter
		for (auto ent : this->paramlist) {
			file << (ent.second)->getLuaString();
			file << "\n";
		}

		// Close file
		file.close();
		return true;
	}
	else {
		printf("Unable to open file\n");
		return false;
	}
}
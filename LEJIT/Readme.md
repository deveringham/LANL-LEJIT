## LEJIT - Lua Easy Just In Time library 
__Version 1.1 - Los Alamos National Laboratory__

__Dylan Everingham - 08/26/2016__             

This is a mostly finished (but always expanding) implementation of the LEJIT library.
The purpose of LEJIT is to enable to configuration of behavior and variables in large physics codes
written in C, C++ or FORTRAN using a script file written in Lua. Because Lua can be compiled just-in-time,
this allows for the changing of the physics codes without expensive recompilations, with minimal performance
losses in many cases.
It includes:
	
* The library files:
 * Lejit.hpp, Lejit.h, Lejit.cpp
 * Param.hpp, Param.cpp
 * LuaUtil.hpp, LuaUtil.cpp
* A sample Makefile:
 * Makefile
* The required Torch package (includes LuaJIT):
 * torch/*
* And finally this readme:
 * Readme.md

Look in the repo's included example_c and example_c++ folders for demonstrations 
of how a code physicist might use the LEJITReader class with LEJIT's C and C++ 
interfaces. Below is a walkthrough showing how to set up a single configurable 
parameter, int my_param, with the C++ interface:

1. Create a new LEJITReader, passing a lua config filename to use to the constructor (default is "config.lua" if none provided):	
 `LEJITReader lr = new ParamReader("myconfig.lua");`

2. Declare the value you want to be configurable and give it a default value:	
 `int my_param = 42;`

3. Register the parameter with the ParamReader, giving it an identifier in the config file:		
 `lr->registerParam("my_param", my_param);`

4. Write the Lua config file. If the config file already exists, this does nothing:		
 `lr->writeConfigFile();`

5. If you ever want to access the value of the parameter in the config file, use readParam():	
 `lr->readParam("my_param", my_param);`

A user can then edit the parameter in the config file at any time:	
 `my_param = 42`

In this way, the very first time the application is run a Lua config file is generated containing a declaration of my_param set to its default value.

If you ever want to change that parameter, simply change its value in the config file and the next time the application is run it will pull the new value with readParam(). You do not need to recompile the code to see the new value, as the Lua config file will be JIT compiled at runtime.

Check out the repo's performance directory for performance tests you can run yourself.

__-Dylan Everingham__

### CURRENT BUGS / OMMISSIONS / ISSUES:
- Currently no way to turn on copy-elements mode for configurable function parameter array arguments
- Currently function parameters must return by out parameters
- Error messages should be made more robust
- Bug fixing with FORTRAN interface
- As always, the code can be made much more elegant and streamlined in a number of places. This is an ongoing process.

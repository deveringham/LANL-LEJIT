 #
 # 	 _____     ________     _____  _____  _________  
 #	|_   _|   |_   __  |   |_   _||_   _||  _   _  | 
 # 	  | |       | |_ \_|     | |    | |  |_/ | | \_| 
 # 	  | |   _   |  _| _  _   | |    | |      | |     
 #	 _| |__/ | _| |__/ || |__' |   _| |_    _| |_    
 #	|________||________|`.____.'  |_____|  |_____|   
 #                                                 
 #			  Lua Easy Just In Time Library
 #						Version 1.0
 #			  Los Alamos National Laboratory
 #
 # Dylan Everingham 08/26/2016
 # Makefile
 #
 # Makefile to compile LEJIT into a shared library liblejit.so
 # See usage examples for how to compile codes with LEJIT
 # using the library
 #

CC=g++ -std=c++11
C=gcc
LUA=-I./torch/install/include -L./torch/install/lib -lluajit -pagezero_size 10000 -image_base 100000000

all: liblejit.so

Lejit.o: Lejit.hpp Lejit.h Lejit.cpp Param.hpp Param.cpp LuaUtil.hpp LuaUtil.cpp 
	$(CC) -o Lejit.o -c Lejit.cpp -Wall -I./torch/install/include 

liblejit.so: Lejit.o
	$(CC) -shared -fPIC Lejit.o ./torch/install/lib/libluajit.dylib -o liblejit.so

planetsim: liblejit.so planetsim.cpp
	$(CC) -o planetsim planetsim.cpp -L. -llejit -L./torch/install/lib -lluajit

clean:
	rm *.o *.so
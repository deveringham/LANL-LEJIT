CC=g++ -std=c++11
C=gcc
LUA=-I$(LEJITPATH)/torch/install/include -L$(LEJITPATH)/torch/install/lib -lluajit -pagezero_size 10000 -image_base 100000000
LEJITPATH=../LEJIT/
SRC=$(LEJITPATH)Lejit.hpp $(LEJITPATH)Lejit.h $(LEJITPATH)Lejit.cpp $(LEJITPATH)Param.hpp $(LEJITPATH)Param.cpp $(LEJITPATH)LuaUtil.hpp $(LEJITPATH)LuaUtil.cpp

all: $(LEJITPATH)liblejit.so
	$(CC) -o planetsim planetsim.cpp -Wall -lm $(LUA)

$(LEJITPATH)Lejit.o: $(SRC)
	$(CC) -o $(LEJITPATH)Lejit.o -c $(LEJITPATH)Lejit.cpp -Wall -I$(LEJITPATH)/torch/install/include 

$(LEJITPATH)liblejit.so: $(LEJITPATH)Lejit.o
	$(CC) -shared -fPIC Lejit.o $(LEJITPATH)/torch/install/lib/libluajit.dylib -o $(LEJITPATH)liblejit.so 

clean:
	rm *.o
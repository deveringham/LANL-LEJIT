LUA=-I$(LEJITPATH)/torch/install/include -L$(LEJITPATH)/torch/install/lib -lluajit -pagezero_size 10000 -image_base 100000000
C=g++ -std=c++11
LEJITPATH=../LEJIT/
SRC=$(LEJITPATH)Lejit.hpp $(LEJITPATH)Lejit.h $(LEJITPATH)Lejit.cpp $(LEJITPATH)Param.hpp $(LEJITPATH)Param.cpp $(LEJITPATH)LuaUtil.hpp $(LEJITPATH)LuaUtil.cpp

all: libperformancetest.so
	$(C) -o performancetest performancetest.cpp -Wall -lm $(LUA)

libperformancetest.so:
	$(C) -c -Wall -Werror -fpic performancetest.cpp -I$(LEJITPATH)/torch/install/include 
	$(C) -shared -o libperformancetest.so performancetest.o -L$(LEJITPATH)/torch/install/lib -lluajit

clean:
	rm *.o *.so
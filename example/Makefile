CXX = g++
CXXFLAGS =  -g -Wall -Werror  -fno-strict-aliasing 
LIB = -L../lib
INCLUDE = -I../include

all : hello
listener.o : listener.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

hello : listener.o hello.cpp
	$(CXX) $(CXXFLAGS) -stdlib=libstdc++ hello.cpp listener.o -o hello $(LIB) -levent_plus $(INCLUDE)
clean:
	-rm -rf *.o
	-rm -rf hello
	-rm -rf *.dSYM


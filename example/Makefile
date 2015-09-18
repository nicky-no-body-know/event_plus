CXX = g++
CXXFLAGS =  -g -Wall -Werror  -fno-strict-aliasing 
LIB = ../lib
INCLUDE =../include

all : hello
listener.o : listener.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE)

hello : listener.o hello.cpp
	$(CXX) $(CXXFLAGS) -stdlib=libstdc++ hello.cpp listener.o -o hello -L$(LIB) -levent_plus -I$(INCLUDE)
clean:
	-rm -rf *.o
	-rm -rf hello


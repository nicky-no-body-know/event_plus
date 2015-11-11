CXX = clang++
CXXFLAGS = -stdlib=libstdc++ -g -fno-strict-aliasing -Wall -Werror
INCLUDE = -I../include
LIB = -L../src -levent_plus.o
TARGET = event_test

all : $(TARGET)

event_test : event_test.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) ../src/event_plus.o $^ -o $@

clean : 
	rm -rf $(TARGET) *.o

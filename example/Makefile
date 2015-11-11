CXX = g++
CXXFLAGS = -g -fno-strict-aliasing -Wall -Werror -stdlib=libc++
INCLUDE = -I../include
LIB = -L../src -levent_plus.o
TARGET = event_test

all : $(TARGET)

event_test : event_test.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIB) $^ -o $@

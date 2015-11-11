CXX = g++
CXXFLAGS = -g -fno-strict-aliasing -Werror -Wall -stdlib=libstdc++
INCLUDE = -I../include
TARGET = event_plus.o
all : $(TARGET)
event_plus.o : event_plus.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	rm -rf *.o

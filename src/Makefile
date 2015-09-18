CXX = clang
INCLUDE = ../include/
AR = ar
LIB = ../lib
CPPFLAGS += -std=c++11 -c -g -Werror -Wall -Wc++98-compat -fno-strict-aliasing 
ARFLAGS = -r

OBJS = backends.o event_base.o event_util.o  event.o log.o
all : libevent_plus.a

%.o : %.cpp
	$(CXX) $(CPPFLAGS) $< -I$(INCLUDE)  -o $@

libevent_plus.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	-mv libevent_plus.a $(LIB)
	#-rm -rf *.o

clean:
	-rm -rf *.o
	-rm -rf $(LIB)/libevent_plus.a

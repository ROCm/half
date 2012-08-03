CC       = /usr/bin/g++
CFLAGS	 = -std=c++0x -Wall -g -O2
CPPFLAGS = -I../include -DHALF_ENABLE_HASH -DHAVE_CPP11_MATH
LDFLAGS  = -lm

test: test.cpp ../include/half.hpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bin/test test.cpp

clean:
	rm -rvf *~


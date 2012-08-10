CC       = /usr/bin/g++
CFLAGS	 = -std=c++0x -Wall -pedantic -g -O2
CPPFLAGS = -I../include
LDFLAGS  = 

test: test.cpp ../include/half.hpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bin/test test.cpp

asm: test.cpp ../include/half.hpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -S test.cpp

clean:
	rm -rvf *~ *.s

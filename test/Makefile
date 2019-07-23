CC       = /usr/bin/g++
CFLAGS	 = -Wall -Wextra -pedantic -g -O3
CPPFLAGS = -I../include
LDFLAGS  = 

test11: src/test11.cpp ../include/half.hpp
	mkdir -p bin
	$(CC) -std=c++11 $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bin/test src/test11.cpp

test03: src/test03.cpp ../include/half.hpp
	mkdir -p bin
	$(CC) -std=c++03 $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bin/test src/test03.cpp
	
asm: src/test11.cpp ../include/half.hpp
	mkdir -p asm
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -S -o asm/test11.asm src/test11.cpp

clean:
	rm -rvf *~ src/*~ asm bin

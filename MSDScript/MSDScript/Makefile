CXX=c++
CFLAGS= -std=c++17

CXXSOURCE= cmdline.cpp  main.cpp expr.cpp
OBJS = main.o cmdline.o expr.o
HEADERS= cmdline.hpp expr.hpp

CCOMPILE = -c
CLINK = -o

msdscript: $(OBJS)
	$(CXX) $(CFLAGS) main.o cmdline.o expr.o -o msdscript

main.o: main.cpp
	c++ -std=c++17 -c main.cpp

cmdline.o: cmdline.cpp cmdline.hpp catch.h
	c++ -std=c++17 -c cmdline.cpp

expr.o: expr.hpp catch.h
	c++ -std=c++17 -c expr.cpp
clean:
	rm -f *.o msdscript

.PHONY: test
test: msdscript
	./msdscript --test

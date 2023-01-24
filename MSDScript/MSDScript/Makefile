CXX=c++
CFLAGS= -std=c++17

CXXSOURCE= cmdline.cpp  main.cpp expr.cpp
HEADERS= cmdline.hpp expr.hpp

CCOMPILE = -c
CLINK = -o

#run: msdscript
#	./msdscript

output: msdscript

general: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CCOMPILE) $(CXXSOURCE)


msdscript: cmdline.o main.o expr.o catch.h
	$(CXX) cmdline.o main.o expr.o $(CLINK) msdscript

clean:
	rm -rf msdscript *.o


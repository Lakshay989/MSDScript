CXX=c++
CFLAGS= -std=c++11

CXXSOURCE= cmdline.cpp  main.cpp
HEADERS= cmdline.hpp

CCOMPILE = -c
CLINK = -o

#run: msdscript
#	./msdscript

output: msdscript

general: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CCOMPILE) $(CXXSOURCE)

msdscript: cmdline.o main.o
	$(CXX) cmdline.o main.o $(CLINK) msdscript

clean:
	rm -rf msdscript *.o


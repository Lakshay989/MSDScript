CXX=c++
CFLAGS= -std=c++17

CXXSOURCE= cmdline.cpp  main.cpp expr.cpp parse.cpp tests.cpp val.cpp 
OBJS = main.o cmdline.o expr.o parse.o tests.o val.o
TESTOBJS = exec.o test_msdscript.o
HEADERS= cmdline.hpp expr.hpp parse.hpp val.hpp

CCOMPILE = -c
CLINK = -o

msdscript: $(OBJS)
	$(CXX) $(CFLAGS) main.o cmdline.o expr.o tests.o parse.o val.o -o msdscript

main.o: main.cpp
	c++ -std=c++17 -c main.cpp

cmdline.o: cmdline.cpp cmdline.hpp catch.h
	c++ -std=c++17 -c cmdline.cpp

expr.o: expr.cpp expr.hpp catch.h
	c++ -std=c++17 -c expr.cpp

tests.o: tests.cpp catch.h
	c++ -std=c++17 -c tests.cpp

parse.o: parse.cpp parse.hpp expr.hpp
	c++ -std=c++17 -c parse.cpp

val.o: val.hpp val.cpp expr.hpp
	c++ -std=c++17 -c val.cpp

test_msdscript: $(TESTOBJS)
	c++ -std=c++17 $(TESTOBJS) -o test_msdscript

test_msdscript.o: test_msdscript.cpp expr.hpp exec.h exec.cpp
	c++ -std=c++17 -c test_msdscript.cpp



.PHONY: clean 
clean:
	rm -f *.o *.out

.PHONY: test
test: msdscript
	./msdscript --test


.PHONY: interp
interp: msdscript
	./msdscript --interp


.PHONY: print
print: msdscript
	./msdscript --print


.PHONY: prettyprint
prettyprint: msdscript
	./msdscript --pretty-print

.PHONY: testmsdscript
testmsdscript: test_msdscript
	./test_msdscript msdscript


.PHONY: doc
doc: 
	cd ../../Documentation/ && doxygen

.PHONY: showdoc
showdoc:
	cd ../../Documentation/html && open index.html

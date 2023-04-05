CXX = c++
CFLAGS = -std=c++17 -fsanitize=undefined -fno-sanitize-recover=undefined

CXXSOURCE= cmdline.cpp  main.cpp expr.cpp parse.cpp tests.cpp val.cpp env.cpp 
OBJS = main.o cmdline.o expr.o parse.o tests.o val.o env.o
TESTOBJS = exec.o test_msdscript.o
HEADERS= cmdline.hpp expr.hpp parse.hpp val.hpp

CCOMPILE = -c
CLINK = -o

msdscript: $(OBJS)
	$(CXX) $(CFLAGS) main.o cmdline.o expr.o tests.o parse.o val.o env.o -o msdscript

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.hpp catch.h env.hpp
	$(CXX) $(CFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.hpp catch.h pointer.h env.hpp
	$(CXX) $(CFLAGS) -c expr.cpp

tests.o: tests.cpp catch.h env.hpp
	$(CXX) $(CFLAGS) -c tests.cpp

parse.o: parse.cpp parse.hpp expr.hpp
	$(CXX) $(CFLAGS) -c parse.cpp

val.o: val.hpp val.cpp expr.hpp pointer.h
	$(CXX) $(CFLAGS) -c val.cpp

env.o: pointer.h env.hpp val.hpp val.cpp env.cpp
	$(CXX) $(CFLAGS) -c env.cpp

test_msdscript: $(TESTOBJS)
	$(CXX) $(CFLAGS) $(TESTOBJS) -o test_msdscript

test_msdscript.o: test_msdscript.cpp expr.hpp exec.h exec.cpp
	$(CXX) $(CFLAGS) -c test_msdscript.cpp



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

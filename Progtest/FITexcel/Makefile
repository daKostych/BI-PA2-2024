CXX=g++
LD=g++
AR=ar
CXXFLAGS=-std=c++20 -Wall -pedantic -O2 -g
SHELL:=/bin/bash
MACHINE=$(shell uname -m)-$(shell echo $$OSTYPE)

all: test

deps:
	g++ -MM *.cpp > Makefile.d

test: builder.o node.o operators.o functions.o position.o spreadSheet.o test.o
	$(LD) $(CXXFLAGS) -o $@ $^ -L./$(MACHINE) -lexpression_parser

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib: expression_parser.o
	mkdir -p $(MACHINE)
	$(AR) cfr $(MACHINE)/libexpression_parser.a $^

clean:
	rm -f *.o test *~ core sample.tgz Makefile.d

pack: clean
	rm -f sample.tgz
	tar zcf sample.tgz --exclude expression_parser.cpp --exclude Makefile.mingw *


-include Makefile.d
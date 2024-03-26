CXX=g++
LD=g++
CXXFLAGS=-std=c++20 -Wall -pedantic -g
LDFLAGS=

all: input-analyzer

test: input-analyzer
	./input-analyzer

input-analyzer: test.o InputAnalyzer.o
	$(LD) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test.o: test.cpp InputAnalyzer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

InputAnalyzer.o: InputAnalyzer.cpp InputAnalyzer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -f *.o
	rm -f input-analyzer

.PHONY: all clean

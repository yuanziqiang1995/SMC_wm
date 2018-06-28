objects =  CPD.o BayesianModel.o Sampling.o GM.o parser.tab.o checker.o interface.o SampleChecker.o
CC = g++
c = gcc
BISON=bison
BISON_ARGS=-d#-vdt
FLEX=flex
all : SampleChecker SMC_wm
SampleChecker : $(objects) 
	g++ -o SampleChecker $(objects)
SMC_wm : SMC_wm.cpp
	$(CC) -o SMC_wm SMC_wm.cpp -fopenmp -lgsl -lgslcblas
SampleChecker.o : SampleChecker.cpp CPD.h BayesianModel.h Sampling.h GM.h interface.hpp
	$(CC) -std=c++11 -c SampleChecker.cpp
CPD.o : CPD.cpp 
	$(CC) -c CPD.cpp
BayesianModel.o : BayesianModel.cpp CPD.h 
	$(CC) -std=c++11 -c BayesianModel.cpp
Sampling.o : Sampling.cpp BayesianModel.h CPD.h 
	$(CC) -std=c++11 -c Sampling.cpp
GM.o : GM.cpp Sampling.h BayesianModel.h CPD.h 
	$(CC) -std=c++11 -c GM.cpp
interface.o : interface.cpp  checker.hpp Sampling.h BayesianModel.h GM.h CPD.h
	$(CC) -std=c++11 -c interface.cpp
checker.o: checker.cpp checker.hpp Makefile
	$(CC) -c checker.cpp
parser.tab.o: parser.tab.cpp Makefile
	$(CC) -c parser.tab.cpp
parser.tab.cpp: parser.ypp parser.hpp lex.yy.c
	$(BISON) $(BISON_ARGS) parser.ypp

lex.yy.c: lexer.l
	$(FLEX) lexer.l

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f parser.tab.*
	rm -f parser.output
	rm -f SampleChecker
	rm -f SMC_wm
	rm -f core


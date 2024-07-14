#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: pow2 pow2.Tsteps graphs

graphs: pow2.Tsteps pow2.gpi
	gnuplot pow2.gpi

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	 

pow2.Tsteps: pow2
	./pow2 > pow2.Tsteps

tar: pow2.cc pow2.gpi makefile
	tar cvfz 48774775D.tar.gz pow2.cc pow2.gpi makefile

clean: 
	rm -f pow2 pow2.Tsteps fit.log pow2*.png *~
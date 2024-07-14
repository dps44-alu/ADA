#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: qs-vs-hs qs-vs-hs.Msteps graphs

graphs: qs-vs-hs.Msteps qs-vs-hs.gnuplot
	gnuplot qs-vs-hs.gnuplot

qs-vs-hs: qs-vs-hs.cc
	g++ ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.Msteps: qs-vs-hs
	./qs-vs-hs > qs-vs-hs.Msteps

tar: qs-vs-hs.cc qs-vs-hs.gnuplot makefile
	tar cvfz 48774775D.tar.gz qs-vs-hs.cc qs-vs-hs.gnuplot makefile

clean: 
	rm -f qs-vs-hs qs-vs-hs.Msteps fit.log quickSort*.png heapSort*.png qs-vs-hs-RA*.png qs-vs-hs-SA*.png qs-vs-hs-RSA*.png *~

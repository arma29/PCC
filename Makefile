all: pmt aho
	 g++ pmt.o aho.o -o pmt -O2 -O3 -std=c++11

pmt: pmt.cpp
	  g++ -c pmt.cpp -o pmt.o -O2 -O3 -std=c++11

aho: ahoCohasick.cpp
	  g++ -c ahoCohasick.cpp -o aho.o -O2 -O3 -std=c++11

clean: pmt.o ahoCohasick.o
	  rm -f -r *.o
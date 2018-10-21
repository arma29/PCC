OBJS = pmt.cpp ahoCohasick.o boyerMoore.o sellers.o wumanber.o shiftOr.o
CC = g++
CFLAGS = -c
LFLAGS =
VFLAGS = -std=c++11 -O2 -O3

all: pmt clean

pmt: $(OBJS)
		@mkdir -p ../bin/
		$(CC) $(LFLAGS) $(VFLAGS) $(OBJS) -o ../bin/pmt

ahoCohasick.o: ahoCohasick.cpp ahoCohasick.h
		$(CC) $(CFLAGS) $(VFLAGS) ahoCohasick.cpp

boyerMoore.o: boyerMoore.cpp boyerMoore.h
		$(CC) $(CFLAGS) $(VFLAGS) boyerMoore.cpp

sellers.o: sellers.cpp sellers.h
		$(CC) $(CFLAGS) $(VFLAGS) sellers.cpp

wumanber.o: wumanber.cpp wumanber.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) wumanber.cpp

shiftOr.o: shiftOr.cpp shiftOr.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) shiftOr.cpp

clean:
	  @rm -f *.o

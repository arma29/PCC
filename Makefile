OBJS = pmt.cpp ahoCohasick.o boyerMoore.o sellers.o wumanber.o
CC = g++
CFLAGS = -c
LFLAGS = -I .
VFLAGS = -std=c++11 -O2

all: pmt clean

pmt: $(OBJS)
		#@mkdir -p ../bin/
		$(CC) $(LFLAGS) $(VFLAGS) $(OBJS) -o pmt # ../bin/pmt -> cria na pasta bin o exe pmt

ahoCohasick.o: ahoCohasick.cpp ahoCohasick.h
		$(CC) $(CFLAGS) $(VFLAGS) ahoCohasick.cpp

boyerMoore.o: boyerMoore.cpp boyerMoore.h
		$(CC) $(CFLAGS) $(VFLAGS) boyerMoore.cpp

sellers.o: sellers.cpp sellers.h
		$(CC) $(CFLAGS) $(VFLAGS) sellers.cpp

wumanber.o: wumanber.cpp wumanber.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) wumanber.cpp

clean:
	  @rm -f *.o

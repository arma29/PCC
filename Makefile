OBJS = pmt.cpp ahoCohasick.o boyerMoore.o
CC = g++
CFLAGS = -c
LFLAGS =
VFLAGS = -std=c++11 -O2

all: pmt clean

pmt: $(OBJS)
		#@mkdir -p ../bin/
		$(CC) $(LFLAGS) $(VFLAGS) $(OBJS) -o pmt # ../bin/pmt -> cria na pasta bin o exe pmt

ahoCohasick.o: ahoCohasick.cpp ahoCohasick.h
	  $(CC) $(CFLAGS) $(VFLAGS) ahoCohasick.cpp

boyerMoore.o: boyerMoore.cpp boyerMoore.h
		$(CC) $(CFLAGS) $(VFLAGS) boyerMoore.cpp

clean:
	  @rm -f *.o

OBJS = pmt.cpp ahoCohasick.o boyerMoore.o #falta ukk e sellers e dir
CC = g++
CFLAGS = -c
LFLAGS =
VFLAGS = -std=c++11 -O2

all: pmt clean

pmt: $(OBJS)
		@mkdir -p ../bin/
		$(CC) $(LFLAGS) $(VFLAGS) $(OBJS) -o ../bin/pmt

ahoCohasick.o: ahoCohasick.cpp ahoCohasick.h
	  $(CC) $(CFLAGS) $(VFLAGS) ahoCohasick.cpp

boyerMoore.o: boyerMoore.cpp boyerMoore.h
		$(CC) $(CFLAGS) $(VFLAGS) boyerMoore.cpp

clean:
	  @rm -f *.o

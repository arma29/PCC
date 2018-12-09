OBJS = main.cpp sarray.o
CC = g++
CFLAGS = -c
LFLAGS =
VFLAGS = -std=c++11 -O2 -O3

all: ipmt clean

ipmt: $(OBJS)
		@mkdir -p ../bin/
		$(CC) $(LFLAGS) $(VFLAGS) $(OBJS) -o ../bin/ipmt

sarray.o: sarray.cpp sarray.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) sarray.cpp

clean:
	  @rm -f *.o

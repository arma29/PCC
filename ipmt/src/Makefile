OBJS = main.cpp sarray.o lz78.o lz78_trie.o util.o
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

lz78.o: lz78.cpp lz78.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) lz78.cpp

lz78_trie.o: lz78_trie.cpp lz78_trie.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) lz78_trie.cpp

util.o: util.cpp util.h
		$(CC) $(CFLAGS) $(VFLAGS) $(LFLAGS) util.cpp

clean:
	  @rm -f *.o

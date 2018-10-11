all-win: main
	 gcc main.o -o main.exe

all:  main
      gcc main.o -o main

main: main.c
	  gcc -c main.c

clean: main.o
	   del -f *.o

clean-win: main.o
	   rm -f -r *.o
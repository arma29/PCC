all: main
	 gcc main.o -o main.exe

main: main.c
	  gcc -c main.c

clean: main.o
	   del -f *.o
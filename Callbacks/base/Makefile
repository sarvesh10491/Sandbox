# the compiler: gcc for C program
main: main.o callback.o
	gcc -Wall -o main main.o callback.o

main.o: main.c
	gcc -c main.c callback.h

callback.o: callback.c
	gcc -c callback.c callback.h
	
clean:
	rm -f main *.o
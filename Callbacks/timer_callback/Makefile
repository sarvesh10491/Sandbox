# the compiler: gcc for C program
main: main.o timercb.o
	gcc -Wall -o main main.o timercb.o -pthread -lm -lrt

main.o: main.c
	gcc -c main.c timercb.h

timercb.o: timercb.c
	gcc -c timercb.c timercb.h
	
clean:
	rm -f main *.o
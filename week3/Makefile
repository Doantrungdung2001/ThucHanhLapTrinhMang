CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

run: resolver.o
	${CC} resolver.o -o run

resolver.o: resolver.c
	${CC} ${CFLAGS} resolver.c

clean:
	rm -f *.o *~

CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

run: week1.o
    ${CC} week1.o -o run

week1.o: week1.c
    ${CC} ${CFLAGS} week1.c

clean:
    rm -f *.o *~
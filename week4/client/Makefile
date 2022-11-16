CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

run: client.o
	${CC} client.o -o run

client.o: udp_client.c
	${CC} ${CFLAGS} udp_client.c

clean:
	rm -f *.o *~

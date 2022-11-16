CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

run: server.o
	${CC} server.o -o server

server.o: udp_server.c
	${CC} ${CFLAGS} udp_server.c

clean:
	rm -f *.o *~

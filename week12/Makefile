# week12: server.c client.c
# 	gcc -o server  server.c
# 	gcc -o client client.c
# clean:
# 	rm -f *.o *~
CC = gcc

tcp: tcp_client.o select_server.o
	$(CC) tcp_client.o -o client
	$(CC) select_server.o -o server

tcp.o: tcp_client.c select_server.o
	$(CC) -c tcp_client.c
	$(CC) -c select_server.c

clean: 
	$(RM) tcp_client select_server server client *.o *~ 

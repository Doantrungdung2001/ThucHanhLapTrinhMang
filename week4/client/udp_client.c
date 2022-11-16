/*UDP Echo Client*/
#include <stdio.h> 
#include <stdlib.h>         /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT1 5550
#define SERV_PORT2 7891
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 1024

int main(int argc , char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE], *IPAdress;
	struct sockaddr_in server_addr;
	int bytes_sent,bytes_received1,bytes_received2, sin_size;
	int Port;
	//Step 1: Construct a UDP socket
	if ((client_sock=socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}
	
	//Step 2: Define the address of the servers
	IPAdress = argv[1];
	Port = atoi(argv[2]);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Port);
	server_addr.sin_addr.s_addr = inet_addr(IPAdress);
	
	//Step 3: Communicate with server
	printf("\nInsert string to send:");
	memset(buff,'\0',(strlen(buff)+1));
	fgets(buff, BUFF_SIZE, stdin);
	
	sin_size = sizeof(struct sockaddr);
	
	bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
	if(bytes_sent < 0){
		perror("Error: ");
		close(client_sock);
		return 0;
	}
	if(bytes_sent == 1){
		printf("Wating .......\n");
	}
	bytes_received1 = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0,(struct sockaddr *) &server_addr, &sin_size);
	if(bytes_received1 < 0){
		perror("Error: ");
		close(client_sock);
		return 0;
	}
	buff[bytes_received1] = '\0';
	printf("Reply from server: %s\n", buff);

	bytes_received2 = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0,(struct sockaddr *) &server_addr, &sin_size);
	if(bytes_received2 < 0){
		perror("Error: ");
		close(client_sock);
		return 0;
	}
	buff[bytes_received2] = '\0';
	printf("Reply from server: %s\n", buff);
		
	close(client_sock);
	return 0;
}

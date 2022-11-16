#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 8192




int main(int argc , char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE],username[BUFF_SIZE],password[BUFF_SIZE], *IPAdress;
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received,Port;
	

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	IPAdress =  argv[1];
	Port = atoi(argv[2]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Port);
	server_addr.sin_addr.s_addr = inet_addr(IPAdress);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server			
	while(1){
		//send message
		printf("\nEnter Account :\n");
		memset(username,'\0',(strlen(username)+1));
		fgets(username, BUFF_SIZE, stdin);		
		msg_len = strlen(username);
		if (msg_len == 0) break;
		
		bytes_sent = send(client_sock, username, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
			break;
		}
		
		//receive echo reply
		bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
		if(bytes_received <= 0){
			printf("\nError!Cannot receive data from sever!\n");
			break;
		}else{
			//buff[BUFF_SIZE] = '\0';
			//printf("%s\n",buff);
			printf("Please enter the login password: ");	
			memset(password,'\0',(strlen(password)+1));
			fgets(password, BUFF_SIZE, stdin);		
			msg_len = strlen(password);
			if (msg_len == 0) break;
			
			bytes_sent = send(client_sock, password, msg_len, 0);
			if(bytes_sent <= 0){
				printf("\nConnection closed!\n");
				break;
		}
		}
		
	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}

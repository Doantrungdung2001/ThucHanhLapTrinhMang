#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5500
#define BUFF_SIZE 1024

void send_file(FILE *fp, int sockfd);

int main(int argc ,char *argv[]){
	int client_sock,Port;
	char buff[BUFF_SIZE + 1],*IPAdress ,*name ,*pass,account[100],s2[50];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received ,count =0;
	FILE *fp;
	char statuslogin[3];
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	

	IPAdress = argv[1];
	Port = atoi(argv[2]);
	name = argv[3];
	pass = argv[4];
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Port);
	server_addr.sin_addr.s_addr = inet_addr(IPAdress);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	
	//Step 4: Communicate with server
	//Nối tài khoản và mật khẩu thành một chuỗi để gửi cho client(Tài khoản và mật khẩu ngăn cách với nhau bởi dấu " , ")
	strcpy(account,name);
	strcat(account,",");
	strcpy(s2,pass);
	strcat(account,s2);
	//send username	
	bytes_sent = send(client_sock, account, strlen(account), 0);
	if(bytes_sent < 0)
		perror("\nError: ");
	
	//Nhận trạng thái đăng nhập từ server
	bytes_received = recv(client_sock, statuslogin, strlen(statuslogin), 0);
	if (bytes_received < 0)
		perror("\nError: ");
	else if (bytes_received == 0)
		printf("Connection closed.\n");

	//Kiểm tra trạng thái đăng nhập
	if(strcmp(statuslogin,"1") == 0){
		
		printf("Login is successfull!!\n");
		printf("\n-------START---------\n");
		//Bắt đầu chat
		printf("Me : ");
		fgets(buff, sizeof(buff), stdin);
		//printf("%s",buff);
		bytes_sent = send(client_sock, buff,BUFF_SIZE, 0);
		if(bytes_sent < 0)
			perror("\nError: ");

		memset(buff,'\0',BUFF_SIZE);
		bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
		if (bytes_received < 0)
			perror("\nError: ");
		buff[bytes_received] = '\0';
		count = atoi(buff);
		
		//Nhận đoạn chat của những người khác
		for(int i=1;i<count;i++){
			bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
			if (bytes_received < 0)
				perror("\nError: ");
			buff[bytes_received] = '\0';
			printf("%s\n", buff);
		}
		
		
		
	}else if(strcmp(statuslogin,"2") == 0){
		printf("Account is blocked or inactive!!!!!!\n");
	}else{
		printf("Login is fail!!\n");
	}	
	

	//Step 4: Close socket
	
	close(client_sock);
    printf("[+]Disconnected from the server. \n");
	return 0;
}

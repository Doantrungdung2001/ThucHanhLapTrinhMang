#include <stdio.h> 
#include <stdlib.h>         /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024
#define STATUS1 "File Already Exits!" // Nếu tệp tin đã tồn tại
#define STATUS2 "File Transfer Is Completed......"// Nếu tải thành công và tệp tin chưa tồn tại
#define STATUS3 "Error: File Not Found"// Nếu tệp tin cần tải không tồn tại trên máy chủ
#define STATUS4 "Error: Wrong File Format!" // Nếu định dạnh nhập vào chưa đúng



int main(int argc , char *argv[])
{
 
	int client_sock; /* file descriptors */
	char recv_data[BUFF_SIZE],send_data[BUFF_SIZE],*file_name,*IPAdress;
	int msg_len,bytes_sent, bytes_received;
	struct sockaddr_in server_addr; /* server's address information */ /* client's address information */
	int sin_size,Port;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	IPAdress =  argv[1];
	Port = atoi(argv[2]);
	file_name = argv[3];
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Port);
	server_addr.sin_addr.s_addr = inet_addr(IPAdress);

	//Step 3: Listen request from client
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	
	//Step 4: Communicate with client
	msg_len = strlen(file_name);
	if (msg_len == 0){
		printf("Error !!");
	}else{
		bytes_sent = send(client_sock, file_name, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
		}
	}
	
	do {
        char *buffer     = NULL;
		int buffer_size =0,n;
		FILE *fp = fopen("new.jpg","wb");
        if(fp == NULL){
			error("Error opening file");
		}
         
 		n = read(client_sock, &buffer_size, sizeof(int));	//Write to server
 		if (n < 0) {
 			error("\t Error reading size from Server\n");                 
        }
		printf("%d", buffer_size);
        buffer = malloc(buffer_size);
 		n = read(client_sock, &buffer_size, buffer_size);	//Write to server
 		if (n < buffer_size) {
 			error("\t Error reading size from Server\n");                 
        }
		fwrite(buffer,buffer_size,1,fp);
		fclose(fp);
 	} while(0);	
	bytes_received = recv(client_sock, recv_data, BUFF_SIZE-1, 0);
	if(bytes_received <= 0){
		printf("\nError!Cannot receive data from sever!\n");
	}else{
		recv_data[bytes_received] = '\0';
		printf("%s", recv_data);
	}
	
	close(client_sock);
	
}

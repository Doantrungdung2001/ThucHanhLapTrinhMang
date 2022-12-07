#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BACKLOG 2
#define BUFF_SIZE 8192
#define STATUS1 "File Already Exits!"
#define STATUS2 "File Transfer Is Completed......"
#define SATTUS3 "Error: File Not Found"
#define STATUS4 "Error: Wrong File Format!"

enum errcodes {
    SOCKET_FAIL  = -1,
    READ_FAIL    = -2,
    WRITE_FAIL   = -3,
    INVALID_HOST = -4,
    CONNECT_FAIL = -5
};

typedef struct file{
	char name[BUFF_SIZE];
	char path[BUFF_SIZE];
}file;

file file_img;

int read_image_file(char *file_name, char **buffer, int *sizeof_buffer) {
    int c, i;
    int char_read = 0;
    FILE* fp = fopen(file_name, "rb");

	if (fp == NULL) {
        fprintf(stderr, "\t Error: Wrong File Format! : %s", file_name);
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    *sizeof_buffer = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    *buffer = (char *)malloc(*sizeof_buffer);
    fread(*buffer, *sizeof_buffer, 1, fp);
    return 0;
}
// Kiểm tra xem file có tồn tại không?
int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc , char *argv[]){
	int listen_sock,conn_sock;
	char buff[BUFF_SIZE],*file_name,recv_data[BUFF_SIZE];
	struct sockaddr_in server_addr,client_addr; /* server's address information */
	int bytes_sent, bytes_received,Port,sin_size;
	FILE *fp,*p;

	//Doc file
	fp = fopen("file.txt","r");
	while (feof(fp) == 0){
		fscanf(fp,"%s",file_img.name);
        fgetc(fp);
        fscanf(fp,"%s",file_img.path);
        fgetc(fp);
	}
	fclose(fp);

	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 2: Specify server address
	Port = atoi(argv[1]);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}

	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}

	//accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client_addr, &sin_size)) == -1) 
			perror("\nError: ");
  
		printf("You got a connection from %s\n", inet_ntoa(client_addr.sin_addr) ); /* prints client's IP */
	//receives message from client
		bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
		if (bytes_received <= 0){
			printf("\nConnection closed");
			
		}else{
			recv_data[bytes_received] = '\0';
			//printf("\nReceive: %s ", recv_data);
		}
		do{
			int n;
			int size_of_file = 0;
            char *buffer     = NULL,*temp = NULL,*x = NULL;
			/*
			strcpy(temp,strrev(recv_data)); // Dao nguoc chuoi de lay 3 ki tu dau jpg,...
			if(strcmp(strncpy(x,temp,3),"jpg") !=0 || strcmp(strncpy(x,temp,3),"bmp") !=0 || strcmp(strncpy(x,temp,3),"png") !=0){
				strcpy(buff,STATUS4);
				bytes_sent = send(conn_sock,buff, strlen(buff), 0); 
			}
			if (bytes_sent <= 0){
				printf("\nConnection closed");
				break;
			}
			*/
			if (read_image_file(recv_data, &buffer, &size_of_file)) {
                error("\t Reading Image Failed", READ_FAIL);
                         
            }
			n = write(listen_sock, &size_of_file, sizeof(int));
			if(n < 0){
				error("\t Error writing to server\n",WRITE_FAIL);
				free(buffer);
			}
			n = write(listen_sock,buffer, size_of_file);
			if(n < 0){
				error("\t Error writing to server\n",WRITE_FAIL);
				free(buffer);
			}
		}while (0);
		if(exists("new.jpg") == 1){
			strcpy(buff,STATUS1);
			bytes_sent = send(conn_sock,buff, strlen(buff), 0);			
		}
		close(conn_sock);
	close(listen_sock);
	return 0;
}

/*UDP Echo Server*/
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>    /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024

int main(int argc , char *argv[])
{
 
	int server_sock; /* file descriptors */
	char buff1[BUFF_SIZE],buff2[BUFF_SIZE],temp[BUFF_SIZE],result_digit[100],result_char[100];;
	int bytes_sent1,bytes_sent2, bytes_received1,bytes_received2;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in cliaddr1,cliaddr2; /* client's address information */
	int sin_size,Port,n,m,count=0;
	//Step 1: Construct a UDP socket
	if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}
	
	//Step 2: Bind address to socket
	Port = atoi(argv[1]);
	server.sin_family = AF_INET;         
	server.sin_port = htons(Port);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
	bzero(&(server.sin_zero),8); /* zero the rest of the structure */

  
	if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
		perror("\nError: ");
		exit(0);
	}     
	
	//Step 3: Communicate with clients
	while(1){
		sin_size=sizeof(struct sockaddr_in);
    		
		bytes_received1 = recvfrom(server_sock, buff1, BUFF_SIZE-1, 0, (struct sockaddr *) &cliaddr1, &sin_size);
		
		if (bytes_received1 < 0)
			perror("\nError: ");
		else{
			buff1[bytes_received1] = '\0';
			n =0;
			for(int i=0; i<=strlen(buff1); i++){
			/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
			/* Nếu là số thì lưu vào chuỗi result*/
				if(isdigit(buff1[i])){ 
					result_digit[n] = buff1[i];
					n++;
					result_digit[n] = '\0';
				}
    		}
			m=0; 
			for(int i=0; i<=strlen(buff1); ++i){
			/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
			/* Nếu là số thì lưu vào chuỗi result*/
				if(isalpha(buff1[i])){
					
					result_char[m] = buff1[i];
					m++;
					result_char[m] = '\0';
				}
    		}

			bytes_received2 = recvfrom(server_sock, buff2, BUFF_SIZE-1, 0, (struct sockaddr *) &cliaddr1, &sin_size);
		
			if (bytes_received2 < 0)
				perror("\nError: ");
			else{
				buff1[bytes_received2] = '\0';
				n =0;
				for(int i=0; i<=strlen(buff2); i++){
				/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
				/* Nếu là số thì lưu vào chuỗi result*/
					if(isdigit(buff2[i])){ 
						result_digit[n] = buff2[i];
						n++;
						result_digit[n] = '\0';
					}
				}
				m=0; 
				for(int i=0; i<=strlen(buff2); ++i){
				/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
				/* Nếu là số thì lưu vào chuỗi result*/
					if(isalpha(buff2[i])){
						
						result_char[m] = buff2[i];
						m++;
						result_char[m] = '\0';
					}
				}
			}
			//strcpy(temp,buff);
			
	}
	
	close(server_sock);
	return 0;
}

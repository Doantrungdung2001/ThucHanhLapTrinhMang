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
	char buff[BUFF_SIZE],temp[BUFF_SIZE],result_digit[100],result_char[100];;
	int bytes_sent1,bytes_sent2, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in cliaddr; /* client's address information */
	int sin_size,Port,n,count=0,check_digit =-1, check_char = -1;
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
    		
		bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &cliaddr, &sin_size);
		
		if (bytes_received < 0)
			perror("\nError: ");
		else{
			buff[bytes_received] = '\0';
			n =0;
			for(int i=0; i<=strlen(buff); i++){
			/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
			/* Nếu là số thì lưu vào chuỗi result*/
				if(isdigit(buff[i])){ 
					result_digit[n] = buff[i];
					n++;
					result_digit[n] = '\0';
					check_digit =1;
				}
    		}
			n=0; 
			for(int i=0; i<=strlen(buff); ++i){
			/* Kiểm tra ký tự thứ i trong chuỗi đã cho có phải là số hay không*/
			/* Nếu là số thì lưu vào chuỗi result*/
				if(isalpha(buff[i])){
					
					result_char[n] = buff[i];
					n++;
					result_char[n] = '\0';
					check_char = 1;
				}
    		}
			n =0;
			//printf("[%s:%d]: %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),buff);
			if(check_digit == 1 && check_char ==1){
				printf("[%s:%d]: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),result_digit);
				printf("[%s:%d]: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),result_char);
			}
			else{
				printf("[%s:%d]: %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port),buff);
			}
			
			count++;
		}
		//strcpy(temp,buff);
		if(count == 2)
		{
			/* code */
			bytes_sent1 = sendto(server_sock, result_digit, bytes_received, 0, (struct sockaddr *) &cliaddr, sin_size ); /* send to the client welcome message */
			if (bytes_sent1 < 0)
				perror("\nError: ");
			bytes_sent2 = sendto(server_sock, result_char, bytes_received, 0, (struct sockaddr *) &cliaddr, sin_size ); /* send to the client welcome message */
			if (bytes_sent2 < 0)
				perror("\nError: ");
		}
			
	}
	
	close(server_sock);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <string.h>
int main(int argc, const char * argv[]) {
    //create a socket
    int network_socket,bytes_recived,bytes_send;
    network_socket=socket(AF_INET,SOCK_STREAM,0);
    int Port;
    char *IPAdress;
    
    IPAdress = argv[1];
	Port = atoi(argv[2]);

    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Port);
    server_address.sin_addr.s_addr=inet_addr(IPAdress);
    
    int connection_status = connect(network_socket, (struct sockaddr *) & server_address, sizeof(server_address));
    //check for connection_status
    if(connection_status==-1)
    {
        printf("The connection has error\n\n");
    }
    
    if(connection_status==0)
    {
		char response[256],buff[256];
		//receive data from the server
		bytes_recived=recv(network_socket, response, sizeof(response), 0);
		response[bytes_recived]='\0';
		printf("\n%s\n",response);
		while(1){
		    //char response[256];
			
			memset(response,'\0',256);
			printf("enter a message to server: ");
			fgets(buff,256,stdin);
            buff[strlen(buff)-1]='\0';
            //printf("%ld\n",strlen(buff));
			int bytes_send = send(network_socket,buff, strlen(buff),0);
            bytes_recived =recv(network_socket, response, sizeof(response), 0);
			if (strcmp(response,"break")==0){
                printf("\nExit!!\n");
                break;
            }else if(strcmp(response,"Wrong text format")){
                printf("\n%s\n",response);
            }else{
                printf("\n%s\n", response);
            }
				//print out the server's buff
			
		}
	}
    //close the socket
    close(network_socket);
    
    return 0;
}

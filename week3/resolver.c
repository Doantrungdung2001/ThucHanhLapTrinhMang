#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc , char *argv[]){
    struct hostent *nameHost;
    char* host_name;
    struct in_addr **addr_list_IP;
    int i=0;
    char ip[100];
    char **pAlias;

    host_name = argv[1];
    nameHost = gethostbyname(host_name);
    if (nameHost == NULL) { // do some error checking
        herror("gethostbyname"); // herror(), NOT perror()
        printf("Not found information\n");    
    }else{
        printf("Official name is: %s\n", nameHost->h_name);
        addr_list_IP = (struct in_addr **)nameHost->h_addr_list;
        for(i = 0; addr_list_IP[i] != NULL; i++) {
            if(i ==0){
                strcpy(ip ,inet_ntoa(*addr_list_IP[i]));
                printf(" IP addresses:%s \n", ip);
            }
            strcpy(ip ,inet_ntoa(*addr_list_IP[i]));
            printf(" Alias:%s \n", ip);
        }    
        
    }

}

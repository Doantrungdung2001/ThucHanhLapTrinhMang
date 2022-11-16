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
        exit(1);
    }

    printf("Official name is: %s\n", nameHost->h_name);
    printf("IP addresses: ");
    addr_list_IP = (struct in_addr **)nameHost->h_addr_list;
    for(i = 0; addr_list_IP[i] != NULL; i++) {
        strcpy(ip ,inet_ntoa(*addr_list_IP[i]));
        printf(" %s \n", ip);
    }
    printf("Alias IP: \n");
    for(i=0;nameHost->h_aliases[i];i++){
        printf("%s \n",nameHost->h_aliases[i]);
    }
    

}

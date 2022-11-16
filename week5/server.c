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

typedef struct account{
	char name[BUFF_SIZE];
	char password[BUFF_SIZE];
	int status;
}account;

account AC_STUDENT,CHECK_ACCOUNT;
typedef struct Node
{
	account data;
	struct Node * next;
}Node;

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new node at the end  */
void append(struct Node** head_ref, account new_data)
{
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  
    struct Node *last = *head_ref;  /* used in step 5*/
   
    /* 2. put in the data  */
    new_node->data  = new_data;
  
    /* 3. This new node is going to be the last node, so make next 
          of it as NULL*/
    new_node->next = NULL;
  
    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
       
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
   
    /* 6. Change the next of last node */
    last->next = new_node;
    return;    
}

int main(int argc , char *argv[])
{
 
	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE],send_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size,Port,check = -1,count =0;
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode = NULL;
	FILE *fp;

	//Doc file
	fp = fopen("account.txt","r");
	while (feof(fp) == 0){
		fscanf(fp,"%s",AC_STUDENT.name);
        fgetc(fp);
        fscanf(fp,"%s",AC_STUDENT.password);
        fgetc(fp);
        fscanf(fp,"%d",&AC_STUDENT.status);
        //printf("%s %s %d \n",AC_STUDENT.name,AC_STUDENT.password,AC_STUDENT.status);
		append(&newNode,AC_STUDENT);
	}
	fclose(fp);
/*
	while(newNode ->next != NULL){
		    printf("%-20s  %-11s   %-25d\n",newNode -> data.name, newNode->data.password, newNode->data.status);
		    newNode = newNode->next;

    }
*/
	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 2: Bind address to socket
	Port = atoi(argv[1]);
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}     
	
	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 4: Communicate with client
	while(1){
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size)) == -1) 
			perror("\nError: ");
  
		printf("You got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		//start conversation
		while(1){
			//receives message from client
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
			if (bytes_received <= 0){
				printf("\nConnection closed");
				break;
			}
			else{
				recv_data[bytes_received] = '\0';
				printf("\nReceive:%s ", recv_data);
				check = 1;
				count++;
			}
			printf("%d",count);
			if(count == 1){
				strcpy(CHECK_ACCOUNT.name,recv_data);
				printf("\n%s",CHECK_ACCOUNT.name);
			}
			if(count == 2){
				strcpy(CHECK_ACCOUNT.password,recv_data);
				printf("\n%s",CHECK_ACCOUNT.password);
				
				while(newNode ->next != NULL){
				if(strcmp(newNode->data.name,CHECK_ACCOUNT.name) == 0 && strcmp(newNode->data.password,CHECK_ACCOUNT.password) == 0){
					if(newNode->data.status == 1){
						printf("Dang nhap thanh cong !!");
					}else{
						printf("Dang nhap that bai");
					}
				}else{
					printf("Dang nhap that bai");
				}
				newNode = newNode->next;
				
    			}
				while(newNode ->next != NULL){
					printf("%-20s  %-11s   %-25d\n",newNode -> data.name, newNode->data.password, newNode->data.status);
					newNode = newNode->next;

    			}
			}
			
			//echo to client
			bytes_sent = send(conn_sock,recv_data, bytes_received, 0); /* send to the client welcome message */
			if (bytes_sent <= 0){
				printf("\nConnection closed");
				break;
			}
		
		}//end conversation
		close(conn_sock);	
	}
	
	close(listen_sock);
	return 0;
}

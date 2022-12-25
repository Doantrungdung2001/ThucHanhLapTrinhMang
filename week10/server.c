#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>

#define PORT 5500
#define BACKLOG 20
#define BUFF_SIZE 1024

typedef struct account{
	char name[BUFF_SIZE];
	char password[BUFF_SIZE];
	int status;
}account;
account AC_STUDENT;
typedef struct Node{
	account data;
	struct Node * next;
}Node;

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new node at the end  */
void append(struct Node** head_ref, account new_data){
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

//Tách chuỗi
char *Tachchuoi(char x[]){
	char * p;
    p = strtok(x, ",");
	return x;
}

//Đảo chuỗi
char *Daochuoi(char x[]){
    for(int i = 0; i<strlen(x)/2; i++){
        char temp = x[i];
        x[i] = x[strlen(x)-i-1];
        x[strlen(x)-i-1] = temp;
    }
	return x;
}
// Login

/* Handler process signal*/
void sig_chld(int signo);

/*
* Receive and echo message to client
* [IN] sockfd: socket descriptor that connects to client 	
*/
void echo(int sockfd);
int login(Node *node,char name[],char pass[]);// status = 1 login is succefull, status = -1 -> account is block,else ->  fail
int write_file(int sockfd);

int main(int argc , char *argv[]){
	
	int listen_sock, conn_sock; /* file descriptors */
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	pid_t pid;
	int sin_size , Port,statuslogin,count =0;
	int bytes_sent, bytes_received;
	char buff[BUFF_SIZE],name[50],pass[50],*p,repose_client[BUFF_SIZE],chat_client[BUFF_SIZE],line[BUFF_SIZE];
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode = NULL;
	FILE *fp;

	//Doc file
	fp = fopen("taikhoan.txt","r");
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
	while(newNode  != NULL){
		    printf("%-20s %-20ld %-11s %-11ld  %-25d\n",newNode -> data.name,strlen(newNode -> data.name),newNode->data.password,strlen(newNode -> data.password),newNode->data.status);
		    newNode = newNode->next;

    }
*/
	if ((listen_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		printf("socket() error\n");
		return 0;
	}
	Port = atoi(argv[1]);
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(Port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   

	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listen_sock, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	/* Establish a signal handler to catch SIGCHLD */
	signal(SIGCHLD, sig_chld);

	while(1){
		sin_size=sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size))==-1){
			if (errno == EINTR){
				continue;
			}
			else{
				perror("\nError: ");			
				return 0;
			}
		}
		
		/* For each client, fork spawns a child, and the child handles the new client */
		pid = fork();
		
		/* fork() is called in child process */
		if(pid  == 0){
			close(listen_sock);
			printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
			//received
			bytes_received = recv(conn_sock, buff, BUFF_SIZE, 0); //blocking
			if (bytes_received < 0)
				perror("\nError: ");

			buff[bytes_received] ='\0';

			//Phân tích chuỗi tài khoản và mật khẩu được gửi từ client
			// Tai khoan
			strcpy(name,buff);
			Tachchuoi(name);

			//Mat khau
			strcpy(pass,buff);
			Tachchuoi(Daochuoi(pass));
			Daochuoi(pass);
			
			//printf("%s %ld\n",name,strlen(name));
			//printf("%s %ld\n",pass,strlen(pass));

			//login
			statuslogin =login(newNode,name,pass);
			//printf("%d",statuslogin);

			if(statuslogin == 1){
				//Gửi trạng thái đăng nhập cho client
				strcpy(repose_client,"1");
				bytes_sent = send(conn_sock, repose_client, strlen(repose_client), 0); // echo to the client 
				if (bytes_sent < 0)
					perror("\nError: ");

				//Nhận đoạn chat từ client
				bytes_received = recv(conn_sock, chat_client,BUFF_SIZE, 0); //blocking
				if (bytes_received < 0)
					perror("\nError: ");
				chat_client[bytes_received] ='\0';

				//Ghi vào trong file
				fp = fopen("groupchat.txt", "a+");
				fprintf(fp, "%s:%s", name,chat_client);
				fclose(fp);

				//Gủi lại đoạn chat cho client
				fp = fopen("groupchat.txt", "r");
				printf("\n------Chat box-------\n");
				while (!feof(fp)){
					fgets(chat_client,BUFF_SIZE,fp);
					printf("%s",chat_client);
					count++;
				}
				fclose(fp);
				sprintf(line, "%d\n", count);
				//printf("%s",line);
				bytes_sent = send(conn_sock,line,strlen(line), 0); // echo to the client 
				if (bytes_sent < 0)
					perror("\nError: ");
					
				fp = fopen("groupchat.txt", "r");
				while (!feof(fp)){
					fgets(chat_client,BUFF_SIZE,fp);
					bytes_sent = send(conn_sock,chat_client,BUFF_SIZE, 0); // echo to the client 
					if (bytes_sent < 0)
						perror("\nError: ");
				}
				fclose(fp);

			}else if(statuslogin == 2){
				strcpy(repose_client,"2");
				bytes_sent = send(conn_sock, repose_client, strlen(repose_client), 0); // echo to the client 
				if (bytes_sent < 0)
					perror("\nError: ");
			}else{
				strcpy(repose_client,"3\n");
				bytes_sent = send(conn_sock, repose_client, strlen(repose_client), 0); // echo to the client 
				if (bytes_sent < 0)
					perror("\nError: ");
			}
			//send
			bytes_sent = send(conn_sock, buff, bytes_received, 0); // echo to the client 
			if (bytes_sent < 0)
				perror("\nError: ");
			
			//echo(conn_sock);						
			exit(0);
		}
		
		/* The parent closes the connected socket since the child handles the new client */
		close(conn_sock);
	}
	close(listen_sock);
	return 0;
}

void sig_chld(int signo){
	pid_t pid;
	int stat;
	
	/* Wait the child process terminate */
	while((pid = waitpid(-1, &stat, WNOHANG))>0)
		printf("\nChild %d terminated\n",pid);
}

int login(Node *node,char name[],char pass[]){	
	int count=0;
	int status;
	while (node != NULL){
		if(strcmp(node->data.name,name) == 0){
			//printf(" tim thay\n");
			if(strcmp(node->data.password,pass) == 0){
				if(node->data.status == 1){
					printf("Login is successfull!!\n");
					status = 1;
					//break;
				}else{
					printf("Account is blocked or inactive!!!!!!\n");
					status = 2;
					break;
				}
			}else{
				printf("Password is not correct!!!\n");
				status = 3;

			}
		}
		node = node->next;
		
	}
	return status;
}



void echo(int sockfd) {
	char name[BUFF_SIZE],pass[BUFF_SIZE];
	int bytes_sent, bytes_received;
	
	bytes_received = recv(sockfd, name, BUFF_SIZE, 0); //blocking
	if (bytes_received < 0)
		perror("\nError: ");
	name[bytes_received] = '\0';
	printf(" %s\n", name);
	
	bytes_sent = send(sockfd, name, bytes_received, 0); // echo to the client 
	if (bytes_sent < 0)
		perror("\nError: ");
			
	close(sockfd);
}
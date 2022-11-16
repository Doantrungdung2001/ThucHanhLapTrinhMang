#include<stdio.h>
#include<string.h>
#include <stdlib.h>
//Link -list

typedef struct account
{
    char name[50];
    char password[50];
    int status;
}account;

account AC_student;

typedef struct Node
{
    account data;
	struct Node *next;
}Node;


Node *Linklist;
int xacnhan =0;


Node  *Add_Linklist(Node *newNode , account ac)
{
    Node *temp = (struct Node*)malloc(sizeof(struct Node));
    newNode -> data = ac;
    newNode -> next = NULL;
    if(temp == NULL)
    {
        
    }
    return newNode;
}
/*
Node *Add_toEnd(struct Node *newNode, account ac)
{
    if (newNode == NULL)
        Add_toEmpty(&newNode, ac);
     
    Node *temp = (Node *)malloc(sizeof(struct Node));
 
    temp -> data = ac;
    temp -> next = newNode -> next;
    newNode -> next = temp;
    newNode = temp;
 
    return newNode;
}
*/

void Register(Node *newNode,char x[] , char y[]){   
    newNode = (Node*) malloc(sizeof(Node));
    while (newNode -> next != NULL)
    {
        printf("%s",newNode ->data.name);
        
        if(strcmp(newNode->data.name,x) == 0)
        {
            printf("Tai khoan nay da ton tai !!!!");
            printf("Ban hay thu voi tai khoan khac !!!!");
        }
        
        else
        {
            strcpy(AC_student.name,x);
            strcpy(AC_student.password,y);
            AC_student.status = 1;
            Linklist = Add_toEnd(newNode,AC_student);
        }
        
    }
}
void Signin(char x[] , char y[]){
    int count = 0;
    Node *newNode = (Node*) malloc(sizeof(Node));
    while (newNode -> next != NULL)
    {
        if(strcmp(newNode->data.name,x)  & strcmp(newNode->data.password,y) == 0)
        {
            printf("Dang nhap thanh cong !!!!");
            xacnhan = 1;
        }
        else
        {
            printf("Ten tai khoan hoac mat khau khong dung !!!!");
            printf("Xin vui long nhap lai !!!!");
            count++;
        }
        if(count >= 3)
        {
            printf("Tai khan da bi khoa !!!!!");
            newNode ->data.status = 0;
        }
    }
}
void Search(account ac, char x[]){
    Node *newNode = (Node*) malloc(sizeof(Node));
    while (newNode -> next != NULL)
    {
        if(strcmp(newNode->data.name,x) == 0)
        {
            printf("Ten tai khoan : %s",newNode->data.name);
            if(newNode->data.status == 1)
                printf("Tai khoan dang hoat dong");
            if(newNode->data.status == 0)
                printf("Tai khoan da bi khoa");
        }
        else if(xacnhan == 1){
            printf("Tai khoan da dang nhap !!!!");
        }         
        else
        {
            printf("Khong co tai khoan nay trong he thong !!");
                
        }
        
    }
}
void Signout(char x[]){
    Node *newNode = (Node*) malloc(sizeof(Node));
    
    while (newNode -> next != NULL)
    {
        if(strcmp(newNode->data.name,x) == 0)
        {
            printf("Dang xuat thanh cong !!!!");
        }
        else
        {
            printf("Ten tai khoan  khong dung !!!!");
            printf("Xin vui long nhap lai !!!!");
        }
    }
}

int main(){

    char enter,confirm;
    int menu;
    char ac_name[50] , ac_password[50];
    FILE *fp;
    Node *newNode = (Node*) malloc(sizeof(Node));
    
    fp=fopen("account.txt","r");
    while(feof(fp) == 0)
    {
        fscanf(fp,"%s",AC_student.name);
        fgetc(fp);
        fscanf(fp,"%s",AC_student.password);
        fgetc(fp);
        fscanf(fp,"%d",&AC_student.status);
        //printf("%s %s %d \n",AC_student.name,AC_student.password,AC_student.status);
        Add_toEmpty(&newNode,AC_student);
    }
    fclose(fp);
/*
        while(newNode ->next != NULL){
		    printf("%-20s  %-11s   %-25d\n",newNode->data.name, newNode->data.password, newNode->data.status);
		    newNode = newNode->next;

        }
*/
        while(1){
            system("/n");
            printf("\nUSER MANAGEMENT PROGRAM");
            printf("\n-----------------------------------");
            printf("\n1.Register");
            printf("\n2. Sign in");
            printf("\n3. Search");
            printf("\n4. Sign out");
            printf("\n5. Exit");
            printf("\nYour choice (1-5): ");
            scanf("%d",&menu);
            scanf("%c",&enter);
            fflush(stdin);
            switch (menu)
            {
                case 1:
                    printf("Tai khoan : ");
                    scanf("%s",ac_name);
                    printf("mat khau: ");
                    scanf("%s",ac_password);
                    getchar();
                    break;
                case 2:   
                    printf("Tai khoan : ");
                    scanf("%s",ac_name);
                    printf("Mat khau: ");
                    scanf("%s",ac_password);
                    Signin(ac_name, ac_password);
                    getchar();
                    break;
                case 3:
                    printf("Tai khoan : ");
                    scanf("%s",ac_name);
                    getchar();
                    break;
                case 4:
                    printf("Tai khoan : ");
                    scanf("%s",ac_name);
                    Signout(ac_name);
                    getchar();
                    break;
                case 5:
                    printf("Ban co chac chan muon thoat khong (y/n):");
                    scanf("%c",&confirm);
                    if(confirm=='y')
                        exit(0);
            }
        }
    
    return 0;
}

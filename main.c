#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
typedef struct {
    char user_name[50];
    char password[20];
    int status;
} elementtype;
typedef struct node node;
typedef struct node {
	elementtype element;
	node *next;
} node;

typedef struct {
  node *root;
  node *cur;
  node *prev;
  node *tail;
} singleList;
singleList list, signedInList;
//Declare function of linked list
void createSingleList(singleList *list);
node *makeNewNode(elementtype e);
node *insertEnd(singleList *list, elementtype e);
node *deleteHead(singleList *list);
node *deleteAtPosition(singleList *list, int position);
void printSingleList(singleList list);
//Declare function
void readDataFromFile(singleList *list);
int searchAccount(singleList list, char username[50]);//if account exist, return 1, else return 0
int checkBlocked(singleList list, char username[50]);//if blocked, return 1
int checkCorrectPassword(singleList list, char username[50], char pass[20]);
void blockAccount(singleList list,char username[50]);
void activateAccount(singleList list,char username[50]);
int changePass(singleList accountList, singleList signedList,char username[50], char pass[20], char new_pass[20]);
int findPosition(singleList signedInList, char username[50]);
int returnStatus(singleList list, char username[50]);
node *extractAccount(singleList list, char username[50]);
//Implement function of linked list
void createSingleList(singleList *list)
{
    (*list).root = (*list).prev = (*list).cur = (*list).tail  = NULL;
}
node *makeNewNode(elementtype e)
{
    node *new = (node *)malloc(sizeof(node));
    new->element = e;
    new->next = NULL;
    return new;
}
node *insertEnd(singleList *list, elementtype e)
{
	node *new = makeNewNode(e);
	if((*list).root == NULL)
	{
        (*list).root = (*list).tail = new;
	}
	else
	{
		(*list).tail->next = new;
		(*list).tail = new;
	}
	return (*list).tail;
}
node *deleteHead(singleList * list)
{
    if((*list).root != NULL)
    {
        node *new = (*list).root;
		(*list).root = (*list).root->next;
		free(new);
	}
    return (*list).root;
}
node *deleteAtPosition(singleList *list, int position){
    if((*list).root == NULL)
    {
        return (*list).root;
    }
    (*list).cur = (*list).prev = (*list).root;
	//Delete head node
	if (position == 0){
        return deleteHead(list);
	}
	(*list).cur = (*list).prev = (*list).root;
	//store previous of to be deleted node
    for (int i = 0; (*list).cur != NULL && i < position - 1; i++)
    {
        (*list).prev = (*list).cur;
        (*list).cur = (*list).cur->next;
    }
    if ((*list).cur == NULL || (*list).cur->next == NULL)// If position is more than number of nodes
    {
        return (*list).cur;
    }
    if ((*list).cur->next->next == NULL){
        (*list).prev =( *list).cur;
        (*list).cur = (*list).cur->next;
        node *new = (*list).cur;
		(*list).cur = (*list).cur->next;
		free(new);
        (*list).tail = (*list).prev;
        (*list).tail->next = NULL;
        return (*list).tail;
    }
    //delete node at pos (next of pos-1)
    (*list).prev = (*list).cur;
    node *next = (*list).cur->next->next;
    free((*list).cur->next);
    (*list).prev->next = next;
    return (*list).cur;
}
void printSingleList(singleList list){
    list.cur = list.root;
	while(list.cur != NULL)
    {
        printf("%s %s %d\n", list.cur->element.user_name, list.cur->element.password, list.cur->element.status);
        list.cur = list.cur->next;
    }
    return;
}
//Implement function
void readDataFromFile(singleList *list){
    elementtype element;
    FILE *fp;
    fp = fopen("account.txt","r");
    char chr = getc(fp);
    int count_lines = 0;
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(fp);
    }
    fseek(fp, 0, SEEK_SET);
    for(int i = 0; i < count_lines; i++){
      fscanf(fp, "%s %s %d", element.user_name, element.password, &element.status);
      insertEnd(list, element);
    }
    fclose(fp);
}
int searchAccount(singleList list, char username[50]){
    list.cur = list.root;
    // if existed , return 1
    // else return 0
    while (list.cur != NULL)
    {
        if (strcmp(list.cur->element.user_name, username) == 0)
            return 1;
        list.cur = list.cur->next;
    }
    return 0;
}
void alterDataOfFile(singleList list){
    FILE *fp;
    fp = fopen("account.txt","w");
    if (fp == NULL){
        printf("No file input\n");
        return;
    }
    list.cur = list.root;
	while(list.cur != NULL)
    {
        fputs(list.cur->element.user_name, fp);
        fputs(" ", fp);
        fputs(list.cur->element.password, fp);
        fputs(" ", fp);
        fprintf(fp, "%d", list.cur->element.status);
        fputs("\n", fp);
        list.cur = list.cur->next;
    }
    fclose(fp);
    return;
}
int checkBlocked(singleList list, char username[50]){
    list.cur = list.root;
	while(list.cur != NULL){
		if (strcmp(list.cur->element.user_name, username) == 0){
			if(list.cur->element.status == 0){
				return 1;
			}
			else{
				return 0;
			}
		}
		list.cur = list.cur->next;
	}
}
int checkCorrectPassword(singleList list, char username[50], char pass[20]){
    list.cur = list.root;
	while(list.cur != NULL){
		if ((strcmp(list.cur->element.user_name, username) == 0) && (strcmp(list.cur->element.password, pass) == 0)){
			return 1;
		}
		list.cur = list.cur->next;
	}
	return 0;
}
void blockAccount(singleList list,char username[50]){
    list.cur = list.root;
	while(list.cur != NULL){
		if (strcmp(list.cur->element.user_name, username) == 0){
			list.cur->element.status = 0;
			return;
		}
		list.cur = list.cur->next;
	}
}
void activateAccount(singleList list,char username[50]){
    list.cur = list.root;
	while(list.cur != NULL){
		if (strcmp(list.cur->element.user_name, username) == 0){
			list.cur->element.status = 1;
			return;
		}
		list.cur = list.cur->next;
	}
}
int returnStatus(singleList list, char username[50]){
    list.cur = list.root;
    while(list.cur != NULL){
        if (strcmp(list.cur->element.user_name, username) == 0){
            return list.cur->element.status;
        }
        list.cur = list.cur->next;
    }
}
node *extractAccount(singleList list, char username[50]){
    list.cur = list.root;
    while(list.cur != NULL){
        if (strcmp(list.cur->element.user_name, username) == 0){
            return list.cur;
        }
        list.cur = list.cur->next;
    }
}
int changePass(singleList accountList, singleList signedList,char username[50], char pass[20], char new_pass[20]){
    signedList.cur = signedList.root;
	while(signedList.cur != NULL){
		if ((strcmp(signedList.cur->element.user_name, username) == 0) && (strcmp(signedList.cur->element.password, pass) == 0)){
            strcpy(signedList.cur->element.password, new_pass);
		}
		signedList.cur = signedList.cur->next;
	}
	accountList.cur = accountList.root;
	while(accountList.cur != NULL){
		if ((strcmp(accountList.cur->element.user_name, username) == 0) && (strcmp(accountList.cur->element.password, pass) == 0)){
            strcpy(accountList.cur->element.password, new_pass);
            return 1;
		}
		accountList.cur = accountList.cur->next;
	}
	return 0;
}
int findPosition(singleList signedInList, char username[50]){
    int position = 0;
    signedInList.cur = signedInList.root;
	while(signedInList.cur != NULL){
		if ((strcmp(signedInList.cur->element.user_name, username) == 0)){
			return position;
		}
		position++;
		signedInList.cur = signedInList.cur->next;
	}
	return position;
}
int main()
{
    FILE *fp;
    fp = fopen("account.txt","r");
    if (fp == NULL){
        printf("No file input\n");
        return 0;
    }
	createSingleList(&list);
	readDataFromFile(&list);
    //Menu
    int choice;
    char input[50];
    int count = 0; //count: mark if '\n' is in buffer or not, if count > 0, then need to delete '\n' in buffer 
    do{
        printSingleList(list);
        printf("****************************\n");
        printf("USER MANAGEMENT PROGRAM\n");
        printf("----------------------------\n");
        printf("1. Register\n");
        printf("2. Active\n");
        printf("3. Sign in\n");
        printf("4. Search\n");
        printf("5. Change password\n");
        printf("6. Sign out\n");
        printf("Your choice (1-6, other to quit): ");
        if (count > 0 )
            while( getchar() != '\n' );
        fgets(input, 50, stdin);
        if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n'))
            input[strlen (input) - 1] = '\0';
        if (strpbrk(input, " ") != NULL){
            printf("Goodbye\n");
            return 0;
        }
        choice = atoi(input);
        switch(choice){
            case 1:{
                char username[50], pass[20];
                printf("\n---------Register---------\n");
                printf("Username: ");
                fgets(username, 50, stdin);
                username[strlen (username) - 1] = '\0';
                if (strpbrk(username, " ") != NULL){
                    printf("Username cannot have space!\n");
                    count++;
                    break;
                }
                //scanf("%s", username);
                if (searchAccount(list, username) == 1){
                    printf("\nAccount existed\n");
                    count = 0;//because no use scanf like else case below, there is no need to delete '\n' in buffer when read input
                    break;
                }
                else{
                    printf("Password: ");
                    scanf("%s", pass);
                    elementtype element;
                    strcpy(element.user_name, username);
                    strcpy(element.password, pass);
                    element.status = 2;
                    insertEnd(&list, element);
                    alterDataOfFile(list);
                    printf("Successful registration. Activation required.\n");
                    count++;
                    break;
                }
            }
            case 2:{
                char username[50], pass[20], code[10];
                int wrong_code_count = 0;
                printf("\n---------Activate---------\n");
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", pass);
                while(1){
                    printf("Activate code: ");
                    scanf("%s", code);
                    if (strcmp("LTM121216", code) == 0){
                        activateAccount(list, username);
                        printf("Account is activated\n");
                        alterDataOfFile(list);
                        break;
                    }
                    else{
                        if (wrong_code_count == 3)
                            break;
                        printf("Account is not activated\n");
                        wrong_code_count++;
                    }
                }
                if (wrong_code_count == 3){
                    printf("Activation code is incorrect. Account is blocked\n");
                    blockAccount(list, username);
                    alterDataOfFile(list);
                }
                count++;
                break;
            }
            case 3:{
                char user_name[50], pass[20];
                printf("\n---------Sign In---------\n");
                printf("Username: ");
                scanf("%s", user_name);
                if (searchAccount(list, user_name)){//check if account exist ?
                    if(checkBlocked(list, user_name) != 1){//check if account is blocked?
                        int wrong_pass_count = 0;
                        while(1){
                            while ((getchar()) != '\n');
                            printf("Password: ");
                            scanf("%s", pass);
                            if(checkCorrectPassword(list, user_name, pass)){//check if pass is correct?
                                printf("Hello %s\n", user_name);
                                elementtype element;
                                strcpy(element.user_name, user_name);
                                strcpy(element.password, pass);
                                element.status = returnStatus(list, user_name);
                                insertEnd(&signedInList, element);//insert signed in account in 
                                break;
							}
							else{
                                if (wrong_pass_count == 3)
                                    break;
                                printf("Password is incorrect\n");
                                wrong_pass_count++;
							}
                        }
                        if (wrong_pass_count == 3){
                            printf("Password is incorrect. Account is blocked\n");
                            blockAccount(list, user_name);
                            alterDataOfFile(list);
                        }
                    }
                    else{
                        printf("Account is blocked\n");
                    }

                }
                else{
                    printf("Cannot find account\n");
				}
                count++;
                break;
            }
            case 4:{
                printf("\n---------Search---------\n");
                char user_name[50];
                printf("Username: ");
                scanf("%s", user_name);
                if (searchAccount(list, user_name)){
                    node *account_pointer = extractAccount(list, user_name);
                    if (account_pointer->element.status == 0){
                        printf("Account is blocked\n");
                    }
                    else if (account_pointer->element.status == 1){
                        printf("Account is active\n");
                    }
                    else{
                        printf("Account is not activated\n");
                    }
                }
                else{
                    printf("Cannot find account\n");
                }
                count++;
                break;
            }
            case 5:{
                char user_name[50], pass[20], new_pass[20];
                printf("\n---------Change Password---------\n");
                printf("Username: ");
                scanf("%s", user_name);
                while ((getchar()) != '\n');
                if (searchAccount(signedInList, user_name)){
                    printf("Password: ");
                    scanf("%s", pass);
                    printf("NewPassword: ");
                    scanf("%s", new_pass);
                    if (changePass(list, signedInList, user_name, pass, new_pass)){
                        printf("Password is changed\n");
                        alterDataOfFile(list);
                    }
                    else
                        printf("Current password is incorrect. Please try again\n");
			    }
                else{
                    printf("Cannot find account\n");
                    count = 0;
                    break;
                }
                count++;
                break;
            }
            case 6:{
                printf("\n---------Sign out---------\n");
                char user_name[50];
                printf("Username: ");
                scanf("%s", user_name);
                if (searchAccount(list, user_name)){
                    if (searchAccount(signedInList, user_name)){
                        printf("Goodbye %s\n", user_name);
                        deleteAtPosition(&signedInList, findPosition(signedInList, user_name));
                    }
                    else{
                        printf("Account is not sign in\n");
                    }
                }
                else{
                    printf("Cannot find account\n");
				}
                count++;
                break;
            }
            default:
                printf("Goodbye\n");
                return 0;
        }
    } while (choice >= 1 && choice <= 6);
    return 0;
}

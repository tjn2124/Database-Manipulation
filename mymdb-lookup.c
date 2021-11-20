#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mylist.h"
#include "mdb.h"

char * mystrncpy(char *dest, const char *src, size_t n);

//created an altered version of strncpy that replaces last char 
//with a null terminator if there string isn't null terminated
//this replaces all strings entered with the last char '\n' with '\0'
char * mystrncpy(char *dest, const char *src, size_t n)
{

	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++){
		dest[i] = src[i];
	}
	dest[i] = '\0';
        return dest;
}
int main(int argc, char** argv)
{

	//if there is no database argument to search exit
	if(argc !=2){
		fprintf(stderr, "%s\n", "usage: mdb-lookup <database_file>");
		exit(1);
	}

	//initialize a list 
	struct List list;
	initList(&list);



	//open database file
	struct Node *node;
	node = NULL; 

	char *filename = argv[1]; 
	FILE *fp = fopen(filename, "rb");

	if(fp == NULL)
	{
		perror(filename);
		exit(1);
	}

	char buff[40];
	int counter = 1;
	int obsRead;

	struct MdbRec *xRec;
 	
	int i = 0;
	char luArgs[24];
	

	//read items from the database file one record at a time into the buffer
	//saving them on heap and saving the whole MdbRec node and add 
	//it to the list 
	while((obsRead = fread(buff, sizeof(char),16, fp))){	
		xRec = malloc(sizeof(char)*40);
		mystrncpy(xRec->name, buff, 16);
		
		if ((obsRead = fread(buff, sizeof(char),24, fp))){
			++i;
			mystrncpy(xRec->msg, buff, 24);
		
			node = addAfter(&list, node, fp);
			node->data= xRec;

			if (node==NULL)
			{
				printf("addAfter() failed");
			}
		}
	}
	fclose(fp);
	
	//create a pointer to an Mbd struct so we can dereference data in nodes
	//and store it in correct type which is struct MdbRec
	struct MdbRec *recData;

	char *match;
	char *match2;
	char copy[1000];
	printf("lookup: ");
	while(fgets(luArgs, sizeof(copy), stdin))
	{
		//reset copy with null terminators
		bzero(copy, 6);
		
		//if input is a newline character print out all records
		if(strlen(luArgs)==1){
			node = list.head;
			while (isEmptyList(&list) != 1 && node!=NULL){
				recData = node->data;
				printf("%4d: {%s} said {%s}\n",counter,(recData->name), recData->msg);
				node = node->next;
				++counter;
			}
			counter = 1;
		 	printf("\n");	
			printf("lookup: ");

		}
		//if input is a word to search for,
		//compare to each record and print if a match
		else if(strlen(luArgs)>1){
			node = list.head;
                        while (isEmptyList(&list) != 1 && node !=NULL){
				recData = node->data;
				strncpy(copy, luArgs, 5);

				//omit the newline in the word the  user entered 
				if(strlen(copy)>0 && copy[strlen(copy)-1] == '\n'){
					copy[strlen(copy)-1] = '\0';
				}

				match = strstr(recData->name, copy);
				
				match2 = strstr(recData->msg,  copy);
				if(match || match2){
					printf("%4d: {%s} said {%s}\n",counter,(recData->name), recData->msg);
				}
				++counter;
				node = node->next;

                         }
			counter = 1;
			printf("\n");
			printf("lookup: ");
		}

	}

	node = list.head;
	while((isEmptyList(&list)!= 1) && node!=NULL){
		xRec = node->data;
		free(xRec);
		node = node->next;
	
	}
	removeAllNodes(&list);
	
}
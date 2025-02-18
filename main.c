/*
DOCUMENTATION:

Name        : Rohit Andani
Date        : 01/12/2024
Project Name: Inverted Search
Description: An inverted index is an index data structure storing a mapping from content, such as words or numbers, to its locations in a database file,             or in a document or a set of documents. The purpose of an inverted index is to allow fast full text searches, at a cost of increased proces             sing when a document is added to the database. The inverted file may be the database file itself, rather than its index. It is the most pop             ular data structure used in document retrieval systems, used on a large scale for example in search engines. The purpose of this project is             to implement the inverted search using Hash Algorithms.
 */
#include "invert.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
	sleep(1);
	print("\nINFO: Arguments be like -> executable_filename file1.txt file2.txt ........\n");
	return FAILURE;
    }

    Flist *head = NULL;
    for(int i=1; i<argc; i++)
    {
      read_and_validate_arguments(argv[i], &head); 
    }
    sleep(1);
   // printf("\nINFO: Read and validation part completed...\n\n");

    Flist *temp = head;
    if(temp == NULL)
    {
	print("\nNo files saved in the list..\n");
    }
    int k=0;
    print("\nFile saved in the list: \n");
    while(temp!=NULL)
    {
	printf("%d --> %s\n",k, temp->file); //printig the files saved in the linked list
	k++;
	temp = temp->link;
    }

    hash arr[28];
    for(int i=0; i<28; i++)
    {
	arr[i].index = i;
	arr[i].link = NULL;
    }

   /* for(int i=0;i<28;i++)
    {
	printf("%d\n",arr[i].index);
    }*/



     //create_database(arr, &head);
    //display_database(arr);

    int c_flag = 0, u_flag=0;
char search[20];
    int i=1;
    while(i)
    {
	print("\n1)CREATE DATABASE\n2)DISPLAY DATABASE\n3)SEARCH DATABASE\n4)SAVE DATABASE\n5)UPDATE DATABASE\n6)EXIT\nEnter your choice:");
	int choice;
	scanf("%d",&choice);

	switch(choice)
	{

	    case 1:
                if(c_flag == 0)//flag shoud be 0 for first time, after creating u can't create again
		{  
		 if(create_database(arr,&head)==SUCCESS) //calling the create database fuctio
		 {
		    print("\nDatabase created successfully\n");
		    c_flag++;
		 }

		 else
		 {
		    print("\nDatabase couldn't be created\n");
		 }
		}
		else
		{
		    print("\nDatabase is already created..\n");
		}
		break;


	    case 2:

		if(display_database(arr)==SUCCESS)//calling the display database function to display all the data in the database
		{
		    print("\nThe above details stored in database\n");
		}

		else
		{
		    print("\nDatabase couldn,t be displayed\n");
		}

		break;


	   case 3:
		print("\nEnter the word to be searched:");
		scanf("%s",search); //asking user to enter the word to be searched
		if(search_database(search,arr)==SUCCESS) //calling the function to search the word in the database
		{
		}
		else
		{
		    print("Word is not found in database\n");
		}
		break;
	   case 4:

		if(save_database(arr)==SUCCESS) //after creating or updating database we are saving in a txt file
		{
		    print("\nDatabase is saved successfully\n");
		}
		else
		{
		    print("\nDatabase couldn't be saved...\n");
		}
		break;
	   case 5:
		if(c_flag == 0 && u_flag == 0)
		{
		  if(update_database(arr)==SUCCESS) //updating database if we are not yet created database
		  {
		    print("\nDatabase is updated successfully\n");
		    u_flag++;
		  }  
		  else
		  {
		    print("\nDatabase couldn't be updated\n");
		  }
		}
		else
		{
		    print("\nDatabase is already created or updated....\n");
		}
		   break;
	   case 6:

		print("\n<<<<<<<<<<<<<<<Thank You>>>>>>>>>>>>>>>\n\n"); //exiting from the program.
		return 0;
		break;

	   default:
		print("\n<<Invalid choice>>\n");
    }
   }
}

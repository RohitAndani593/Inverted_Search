#include "invert.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void print(char *str)//function definition to print with animation
{
    for (char *p = str; *p; p++) 
    {
        putchar(*p);
        fflush(stdout);
        usleep(20000);
    }
}

//after getting the files from the command line, function definition to validate all of them
int read_and_validate_arguments(char *argv, Flist **head)
{
        //checking file is .txt file or not
	if(strstr(argv, ".txt"))
        {
	    //opening the file in read mode
	    FILE *fptr = fopen(argv, "r");
	    if(fptr != NULL)
	    {
               fseek(fptr, 0, SEEK_END);
               //checking file has content or not	       
               if(ftell(fptr)>1)
	       {
		       rewind(fptr);
		       if(*head == NULL)
		       {
			   *head = create_node_from_arguments(argv);
			   return SUCCESS;
		       }
		       else
		       {
			   Flist *temp = *head;
			   while(temp->link != NULL)
			   {
			       //checking the files are repeated or not in command line
			       if(strcmp(temp->file, argv)==0)
			       {
				  print("\nINFO: Duplicate file found\n");
				  sleep(1);
				  return FAILURE;
			       }
			       temp = temp->link;
			   }
			   if(strcmp(temp->file, argv)==0)
			       return FAILURE;
			   temp->link = create_node_from_arguments(argv);
			   return SUCCESS;
		       }

	       }
	       else
	       {
		   printf("\nINFO: %s file have no content\n",argv);
		   sleep(1);
		   return FAILURE;
	       }
	    }
	    else
	    {
		printf("\nINFO: %s file is not present in this directory\n", argv);
		sleep(1);
		return FAILURE;
	    }
	}
	else
	{
            print("\nINFO: You should pass files with .txt extension\n\n");
	    sleep(1);
	    return FAILURE;
	} 
}

//function definition for creating node from arguments
Flist *create_node_from_arguments(char *argv)
{
    Flist *new = malloc(sizeof(Flist));
    if(new == NULL)
	return NULL;
    strncpy(new->file, argv, strlen(argv));
    new->file[strlen(argv)] = '\0';
    new->link = NULL;
    return new;
}

//function definition to create database from the files
int create_database(hash *arr, Flist **head)
{
    for(int i=0; i<28; i++)
    {
	arr[i].link = NULL;
    }
    Flist *temp = *head;
    char word[20];
    int index;
    //running th loop till end or=f the linked list to extract filenames
    while(temp != NULL)
    {
	//opening the files
	FILE *fptr = fopen(temp->file, "r");
//	printf("opened file is %s\n", temp->file);
	//extracting word by word from the file
	while(fscanf(fptr, "%s", word) != EOF)
	{
	    //finding the index of the word to store in database
           if(word[0] >= 'A' && word[0] <= 'Z')
	       index = word[0] - 'A';
	   else if(word[0] >= 'a' && word[0] <= 'z')
	       index = word[0] - 'a';
	   else if(word[0] >= '0' && word[0] <= '9')
	       index = 26;
	   else
	       index = 27;
	   pivot *main_temp = arr[index].link;
	   //loop for creating subnodes to store the filenames where the word is present
	   while(main_temp != NULL)
	   {
	       //checking the word is already there in database or not
	       if(strcmp(main_temp->word, word)==0)
	       {
		   sub *sub_temp = main_temp->sublink;
		   while(sub_temp != NULL)
		   {
		       //checking the filename is already there in database or not
		       if(strcmp(sub_temp->file_name, temp->file) == 0)
		       {
			   sub_temp->word_count++;
			   break;
		       }
		       if(sub_temp->sub_link == NULL)
		       {
			   //creating the subnode for the filename if files are not repeated
                            sub_temp->sub_link = create_sub_node(temp->file);   
			    main_temp->file_count++;
			    break;
		       }
		       sub_temp = sub_temp->sub_link;
		   }
		   break;
	       }
	       main_temp = main_temp->main_link;
	   }
            
	   if(main_temp == NULL)
	   {
	       //after reaching the last node if word is not repeated
	       //creating main node to store that word
	       pivot *new_main = malloc(sizeof(pivot));
	       if(new_main == NULL)
	       {
		   return FAILURE;
	       }
	       //for each main node we have to create one subnode
	       sub *new_sub = malloc(sizeof(sub));
	       if(new_sub == NULL)
		   return FAILURE;
               //copying the read word to the main node
	       strcpy(new_main->word, word);
	       //setting the word count as 1
	       new_main->file_count = 1;
	       new_main->sublink = new_sub;
	       new_main->main_link = arr[index].link;
	       arr[index].link = new_main;
	       //copying the read filename to the subnode which we created before
	       strcpy(new_sub->file_name, temp->file);
	       //setting word count in that particular file as 1
	       new_sub->word_count = 1;
	       new_sub->sub_link = NULL;

	   }
	}
         temp = temp->link;
    }
}

//function definition for creating subnode
sub *create_sub_node(char *filename)
{
    sub *new = malloc(sizeof(sub));
    if(new == NULL)
	return NULL;
    new->sub_link = NULL;
    strncpy(new->file_name, filename, strlen(filename));
    new->file_name[strlen(filename)] = '\0';
    new->word_count = 1;
    return new;
}


//function definition for creating main_node
pivot *create_main_node(char word[], sub *new_sub)
{
    pivot *new = malloc(sizeof(pivot));
    if(new == NULL)
	return NULL;
    strncpy(new->word, word, strlen(word));
    new->word[strlen(word)] = '\0';
    new->main_link = NULL;
    new->file_count = 1;
    new->sublink = new_sub;
    return new;
}

//function definition for displaying the database
int display_database(hash arr[])
{
    int count = 0;
   for(int i=0; i<28; i++)
   {
       if(arr[i].link == NULL)
	   count++;
   }
   //checking database is empty or not
   if(count == 28)
   {
       print("\nDatabase is empty...\n");
       return FAILURE;
   }
   print("\n");
   //printing the data in perfect alignment(table)
   printf("----------------------------------------------------------------------------------\n");
   printf("| %-6s | %-20s | %-10s | %-20s | %-10s |\n", "Index", "Word", "File count", "File Name", "Word Count");
   printf("----------------------------------------------------------------------------------\n");
   for(int i=0; i<28; i++)
   {
       if(arr[i].link!=NULL)
       {
	  // printf("[%d]===>    ", i);
	   pivot *temp = arr[i].link;
	   while(temp != NULL)
	   {
	      sub *sub_temp = temp->sublink;
	      int k=0;
	      while(sub_temp != NULL)
	      {
		  if(k==0)
		  {
		      printf("| %-6d | %-20s | %-10d | %-20s | %-10d |\n", i, temp->word, temp->file_count, sub_temp->file_name, sub_temp->word_count);
		      k++;
		  }
		  else
		      //for word is present in more files
                      printf("|        |                      |            | %-20s | %-10d |\n", sub_temp->file_name, sub_temp->word_count);
		  sub_temp = sub_temp->sub_link;
	      }
     printf("----------------------------------------------------------------------------------\n");
	      temp = temp->main_link;
	   }
       }
      
   }
   printf("\n");
   return SUCCESS;
}

//function definition for updating the databse
int update_database(hash arr[]) {
    char file[15];
    printf("\nEnter the file name: ");
    //reading filename from that we have to update databse
    scanf("%s", file);

    //checking the file is .txt file or not
    if (strlen(file) < 4 || strcmp(file + strlen(file) - 4, ".txt") != 0)
    {
        printf("\nEnter the file with .txt extension\n");
        return FAILURE;
    }
    
    //opening that file in read mode to extract the informatioon line by line
    FILE *fptr = fopen(file, "r");
    //checking the file is existing or not
    if (fptr == NULL) {
        printf("\nFile does not exist\n");
        return FAILURE;
    }
    
    //checking the first character of the file
    if (fgetc(fptr) != '#') {
        printf("\nInvalid file format: it should contain # at the beginning\n");
        fclose(fptr);
        return FAILURE;
    }

    //setting seek position to the first
    rewind(fptr);
    
    //variables to store the read data from the .txt file
    int index, word_count, file_count;
    char word[20], file_name[20];

    //readng the index, word, and filecount
    while (fscanf(fptr, "#%d;%[^;];%d;", &index, word, &file_count) == 3) 
    {
        pivot *temp = arr[index].link;
        if (temp == NULL)
       	{
            arr[index].link = malloc(sizeof(pivot));
            temp = arr[index].link;
        } 
	else
       	{
            while (temp->main_link != NULL)
                temp = temp->main_link;
            temp->main_link = malloc(sizeof(pivot));
            temp = temp->main_link;
        }
        //copying the read word to the main node
        strcpy(temp->word, word);
	//setting the filecount 
        temp->file_count = file_count;
        temp->main_link = NULL;
        temp->sublink = NULL;
        
	//running the loop to get the file name and word count
        for (int i = 0; i < file_count; i++) 
	{
	    //readng the filename and the word count
            if(fscanf(fptr, "%[^;];%d;", file_name, &word_count) != 2)
                break;

            sub *new = malloc(sizeof(sub));
	    //copying the file name to the sub_node
            strcpy(new->file_name, file_name);
	    //copying the word count to sub node
            new->word_count = word_count;
            new->sub_link = NULL;

            if (temp->sublink == NULL) 
	    {
                temp->sublink = new;
            } 
	    else
	    {
                sub *sub_temp = temp->sublink;
                while (sub_temp->sub_link != NULL)
                    sub_temp = sub_temp->sub_link;
                sub_temp->sub_link = new;
            }
        }
	fscanf(fptr, "#\n");
    }

    fclose(fptr);
    //print("\nDatabase updated successfully..\n");
    return SUCCESS;
}

//function definition searching the word in the database
int search_database(char word[],hash arr[])
{
   int count = 0;
   for(int i=0; i<28; i++)
   {
       if(arr[i].link == NULL)
	   count++;
   }
   //checking database is empty or not
   if(count == 28)
   {
       print("\nDatabase is empty...\n");
       return FAILURE;
   }
   print("\n");
    for(int i=0;i<28;i++)
    {
	if(arr[i].link!=NULL)
	{
	    //temporary pointer to traverse through the main node after finding the index
	    pivot *temp=arr[i].link;
	    if(temp==NULL)
	    {
		print("\nData base is empty\n");
		return FAILURE;
	    }
	    while(temp!=NULL)
	    {
		//f word found in the database, returning SUCCESS
		if(strcmp(temp->word,word)==0)
		{
		    printf("Information regarding your search: \n");
		    printf("-----------------------------------------------------------------------\n");
		    printf("| %-5s | %-15s | %-9s | %-15s | %-9s |\n", "Index", "Word", "File count", "File name", "Word count");
		    printf("-----------------------------------------------------------------------\n");
		    printf("| %-5d | %-15s | %-9d  | ", i, temp->word, temp->file_count); 
		    sub *sub_temp =  temp->sublink;
		    int i=0;
		    while(sub_temp != NULL)
		    {
			if(i==0)
			{
			    printf("%-15s | %-9d  |\n", sub_temp->file_name, sub_temp->word_count);
			    i++;
			}
			else
			{
			    printf("|       |                 |            | %-15s | %-9d  |", sub_temp->file_name, sub_temp->word_count);
			}
		        sub_temp = sub_temp->sub_link;	
		    }
		    printf("\n-----------------------------------------------------------------------\n");
		    return SUCCESS;
		}
		temp=temp->main_link;
	    }
	}
    }
    //if word not found returning failure
    return FAILURE;
}

//function defintion to save the database in a text file in a particular format
int save_database(hash arr[])
{
    int count=0;
    for(int i=0;i<28;i++)
    {
	if(arr[i].link==NULL)
	    count++;
    }
 
    //checking databse is empty or not
    if(count==28)
    {
	print("\nDatabase is empty\n");
	return FAILURE;
    }

    char file[20];
    print("\nEnter a backup file to save database:");
    //taking one backup file from the user to save the database
    scanf("%s",file);
    //checking the read file is with .txt or not
    if(strstr(file,".txt")==NULL)
    {
	strcpy(file,"backup.txt");
	print("\nBackup file is not passed correctly\n");
	//if read file is not correct creating one default file
	print("\n Default backup file name is 'backup.txt'\n");
    }

    //opening the file in the write mode to print all the data to the file
    FILE *fptr=fopen(file,"w");
    for(int i=0;i<28;i++)
    {
	if(arr[i].link!=NULL)
	{
	    pivot *main_temp=arr[i].link;
	    while(main_temp !=NULL)
	    {
		//printing the index, word, and filecount to the file
 		fprintf(fptr,"#%d;%s;%d;",i,main_temp->word,main_temp->file_count);

	       sub *sub_temp=main_temp->sublink;
	       while(sub_temp != NULL)
	       {
		   //based on filecount printing the filename and wordcount to the .txtx file
		   fprintf(fptr, "%s;%d;", sub_temp->file_name, sub_temp->word_count);
		   sub_temp = sub_temp->sub_link;
	       }
	     //after printing all the data of a word, ending with #\n
	     fprintf(fptr,"#\n");
	     main_temp=main_temp->main_link;
	    }
	}
    }
    return SUCCESS;	    
}



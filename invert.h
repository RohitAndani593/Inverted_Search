#ifndef INVERT_H
#define INVERT_H

#define SUCCESS 0
#define FAILURE -1

//node for storing the file names
typedef struct node
{
    char file[25];
    struct node *link;
}Flist;

//subnode for storing the filename and the wordcount
typedef struct sub_node
{
    int word_count;
    char file_name[20];
    struct sub_node *sub_link;
}sub;

//main node for storing the word and the filecount
typedef struct main_node
{
    char word[20];
    int file_count;
    struct main_node *main_link;
    struct sub_node *sublink;
}pivot;

//hash table for each alphabet word, number word and special char word
typedef struct hash_table
{
    int index;
    struct main_node *link;
}hash;


//all function prototypes

void print(char *str);

int read_and_validate_arguments(char *argv, Flist **head);

Flist *create_node_from_arguments(char *argv);

int create_database(hash arr[], Flist **head);

sub *create_sub_node(char *filename);

pivot *create_main_node(char *word, sub *new_sub);

int display_database(hash arr[]);

int search_database(char *search, hash arr[]);

int save_database(hash arr[]);

int update_database(hash arr[]);

#endif

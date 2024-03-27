#ifndef inverted_search
#define inverted_search

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define SUCCESS 1
#define FAILURE 0
#define EXIST -4

typedef struct filename
{
    char filename[20];
    struct filename *link;
}file_node;

typedef struct subnode
{
    int wordcount;
    char filename[20];
    struct subnode *sublink;
}sub_t;

typedef struct mainnode
{
    int filecount;
    char word[20];
    struct mainnode *mlink;
    struct subnode *slink;
}main_t;

typedef struct hashtable
{
	int data;
	struct mainnode *link;
}hash_t;

int read_and_validate(int argc, char *filename, char *argv[]);

int check_duplicate(int argc, char *argv[], char *filename);
int check_ext(char *filename);
int check_file_exist(char *filename);
int check_file_empty(char *filename);
int check_index(char ch);

int create_hashtable(hash_t *HT);
int create_file_list(char *filename, file_node **head);
main_t *create_mainnode(char *word, char *filename);
sub_t *create_subnode(char *filename);
main_t *create_update_mainnode(char *word, int filecount);
sub_t *create_update_subnode(char *filename, int wordcount);

int create_database(hash_t *arr, file_node **head);
int search_database(hash_t *arr, char *word);
int save_database(hash_t *arr, char *filename);
int validate_update_file(char *filename);
int update_database(hash_t *arr, char *filename);

void print_list(file_node *head);
void display_HT(hash_t *arr, int size);
void display_database(hash_t *arr);

#endif

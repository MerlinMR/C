#include "inverted_search.h"
int i=0, j, flag;
bool database_created = false;
bool database_updated = false;
char filename[20];
FILE *fptr;
file_node *head = NULL;
file_node *head2 = NULL;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf("WARNING: Enter atleast 1 file name.\n");
	printf("\n");
	return FAILURE;
    }

    for(i=1; i <argc; i++)
    {
	if (read_and_validate(argc, argv[i], argv) != FAILURE)
	{
	    //printf("CLEAR: %s meets the required conditions.\n", argv[i]);

	    if (create_file_list(argv[i], &head) == SUCCESS)
	    {
		printf("%s added to file list.\n", argv[i]);
	    }
	    else
	    {
		printf("%s not added to file list.\n", argv[i]);
	    }
	}
	else
	{
	    //printf("WARNING: %s does not satisfy required conditions.\n", argv[i]);
	}
	printf("\n");
    }

    //print_list(head);

    int option;
    hash_t arr[28]; //Declare the hash table array
    create_hashtable(arr);

    printf("Choose the required action from below:\n1. Create Database\n2. Display Database\n3. Update Datebase\n4. Search Database\n5. Save Database\n6. Exit Program\n");
    while (1)
    {
	printf("Choice : ");
	scanf("%d", &option);
	switch (option)
	{
	    case 1:
		if (create_database(arr, &head) == SUCCESS) 
		{
		    printf("Database created successfully.\n");
		}
		else if (create_database(arr, &head) == EXIST)
		{
		    printf("Database already created.\n");
		}
		else
		{
		    printf("Failed to create database.\n");
		    return FAILURE;
		}
		break;

	    case 2:
		display_database(arr);
		break;

	    case 3:
		printf("Enter the file from which database is to be updated: ");
		scanf("%s", filename);
		if (check_ext(filename) == SUCCESS && check_file_exist(filename) == SUCCESS && check_file_empty(filename) == SUCCESS)
		{
		    if (update_database(arr, filename) == SUCCESS)
		    {
			printf("Databased updated successfully.\n");
			//print_list(head2);
			file_node *temp = head2;
			while (temp != NULL)
			{
			    printf("Database updated for %s.\n", temp->filename);
			    temp = temp->link;
			}
		    }
		    else if (update_database(arr, filename) == EXIST)
		    {
			printf("Database already updated.\n");
		    }
		    else
		    {
			printf("Failed to update database.\n");
			return FAILURE;
		    }
		}
		else
		{
		    printf("Enter an existing non-empty file with .txt extention\n");
		}
		break;

	    case 4:
		char word[20];
		printf("Enter the word to be searched: ");
		scanf("%s", word);
		if (search_database(arr, word) != SUCCESS)
		{
		    printf("%s is not present in any file.\n", word);
		}
		break;

	    case 5:
		printf("Enter the file to which the output is to be stored: ");
		scanf("%s", filename);
		if (check_ext(filename) == SUCCESS)
		{
		    save_database(arr, filename);
		    printf("Data base saved to %s successfully.\n", filename);
		}
		else
		{
		    printf("Enter a file with extension .txt");
		}
		break;

	    case 6:
		exit(1);
		break;

	    default:
		printf("Choose a valid option from 1-6.\n");
	}
	printf("\n");
    }

}

int read_and_validate(int argc, char *filename, char *argv[])
{
    if (check_ext(filename) == SUCCESS)
    {
	//printf("%s has correct extension.\n", filename);
	if (check_duplicate(argc, argv, filename) == SUCCESS)
	{
	    //printf("%s has no Duplicates.\n", filename);
	    if (check_file_exist(filename) == SUCCESS)
	    {
		//printf("The file %s exists.\n", filename);
		if (check_file_empty(filename) == SUCCESS)
		{
		    //printf("The file %s is not empty.\n", filename);
		    return SUCCESS;
		}
		else
		{
		    printf("WARNING: %s is empty.\n", filename);
		    return FAILURE;
		}
	    }
	    else
	    {
		printf("WARNING: %s does not exist.\n", filename);
		return FAILURE;
	    }
	}
	else
	{
	    printf("WARNING: Duplicate files (%s) found.\n", filename);
	    return FAILURE;
	}
    }
    else
    {
	printf("WARNING: Only files with extension '.txt' can be used.\n");
	return FAILURE;
    }
}


int check_duplicate(int argc, char *argv[], char *filename)
{
    for (j=i+1;j<argc; j++)
    {
	if (strcmp(argv[j],filename) == 0)
	{
	    return FAILURE;
	}
    }

    return SUCCESS;
}

int check_ext(char *filename)
{
    if(strstr(filename, ".txt") == NULL)
    {
	return FAILURE;
    }
    return SUCCESS;
}

int check_file_exist(char *filename)
{
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
	return FAILURE;
    }
    return SUCCESS;
}

int check_file_empty(char *filename)
{
    fptr = fopen(filename, "r");
    fseek(fptr,0,SEEK_END);
    if(ftell(fptr) == 0)
    {
	return FAILURE;
    }
    return SUCCESS;
}

int create_hashtable(hash_t *HT)
{
    int i=0;
    for(i=0; i<28; i++)
    {
	HT[i].data = -1;
	HT[i].link = NULL;
    }
    return SUCCESS;
}

int create_file_list(char *filename, file_node **head)
{
    file_node *new = malloc(sizeof(file_node));
    if (new == NULL)
    {
	return FAILURE;
    }
    strcpy(new->filename, filename);
    new->link = NULL;

    if (*head == NULL)
    {
	*head = new;
    }
    else
    {
	file_node *temp = *head;
	while(temp->link != NULL)
	{
	    temp = temp->link;
	}
	temp->link = new;
    }
    //printf("------------------------------------------->%s.\n", new->filename);
    return SUCCESS;
}

int check_duplicate_files(char *filename, file_node **head)
{
    file_node *temp = *head;
    while (temp != NULL)
    {
	if (strcmp(temp->filename, filename) == 0)
	{
	    return FAILURE;
	}
	temp = temp->link;
    }
    return SUCCESS;
}

void print_list(file_node *head)
{
    if (head == NULL)
    {
	printf("INFO : List is empty\n");
    }
    else
    {
	while (head)
	{
	    printf("%s -> ", head -> filename);
	    head = head -> link;
	}

	printf("NULL\n");
    }
    //printf("<* FILE LIST CREATED *>\n");
}

int check_index(char ch)
{
    if (isalpha(ch))
    {
	if(isupper(ch))
	{
	    return ch - 'A';
	}
	else if (islower(ch))
	{
	    return ch - 'a';
	}
    }
    else if (isdigit(ch))
    {
	return 27;
    }
    else
    {
	return 26;
    }
}

main_t *create_mainnode(char *word, char *filename)
{
    main_t *new = malloc(sizeof(main_t));
    if (new == NULL)
    {
	printf("Main node not created.\n");
	return FAILURE;
    }
    else
    {
	new->filecount = 1;
	strcpy(new->word, word);
	new->mlink = NULL;
	new->slink = create_subnode(filename);
    }

    return new;
}

sub_t *create_subnode(char *filename)
{
    sub_t *new = malloc(sizeof(sub_t));
    if (new == NULL)
    {
	printf("Sub node not created.\n");
	return FAILURE;
    }
    else
    {
	new->wordcount = 1;
	strcpy(new->filename, filename);
	new->sublink = NULL;
    }
    return new;
}

int create_database(hash_t *arr, file_node **head)
{
    if (database_created)
    {
	return EXIST;
    }
    file_node *temp = *head;
    while (temp != NULL)
    {
	if (check_duplicate_files(temp->filename, &head2) == FAILURE)
	{
	    temp = temp->link;
	    continue;
	}
	//else
	{
	    fptr = fopen(temp->filename,"r");
	    if (fptr == NULL)
	    {
		printf("%s cannot be opened.", temp->filename);
		return FAILURE;
	    }

	    char word[50];
	    while(fscanf(fptr,"%s",word) == 1)
	    {
		int index = check_index(word[0]);
		if(arr[index].link == NULL)
		{
		    arr[index].link = create_mainnode(word, temp->filename);
		    if (arr[index].link == NULL)
		    {
			printf("Could not create main node.\n");
			return FAILURE;
		    }
		}
		else
		{
		    main_t *mtemp = arr[index].link;
		    main_t *mprev = NULL;

		    while (mtemp != NULL)
		    {
			if(strcmp(mtemp->word, word) == 0)
			{
			    sub_t *stemp = mtemp->slink;
			    sub_t *sprev = NULL;
			    while (stemp != NULL)
			    {
				if(strcmp(stemp->filename, temp->filename) == 0)
				{
				    stemp->wordcount = stemp->wordcount + 1;
				    break;
				}
				sprev = stemp;
				stemp = stemp->sublink;
			    }
			    if (stemp == NULL)
			    {
				sprev->sublink = create_subnode(temp->filename);
				mtemp->filecount = mtemp->filecount + 1;
				if (sprev->sublink == NULL)
				{
				    printf("Sub node could not be created.\n");
				    return FAILURE;
				}
			    }
			    break;
			}
			mprev = mtemp;
			mtemp = mtemp->mlink;
		    }
		    if (mtemp == NULL)
		    {
			mprev->mlink = create_mainnode(word, temp->filename);
			if(mprev->mlink == NULL)
			{
			    printf("Mainnode for %s not created.\n", word);
			    return FAILURE;
			}
		    }
		}

	    }
	    printf("Database Created for %s.\n", temp->filename);
	    fclose(fptr);
	    temp = temp->link;
	}
    }
    database_created = true;
    return SUCCESS;
}

void display_database(hash_t *arr)
{
    printf("\n");
    struct mainnode *temp;
    for(i = 0; i < 28; i++)
    {
	if (arr[i].link != NULL)
	{
	    printf("[%d] --> ", i);
	    main_t *mdtemp = arr[i].link;
	    while (mdtemp != NULL)
	    {
		printf("{%s, %d} --> ", mdtemp->word, mdtemp->filecount);
		sub_t *sdtemp = mdtemp->slink;
		while (sdtemp != NULL)
		{
		    printf("|%s : %d| --> ", sdtemp->filename, sdtemp->wordcount);
		    sdtemp = sdtemp->sublink;
		}
		mdtemp=mdtemp->mlink;
	    }
	    printf("\n");
	}
    }
}

int save_database(hash_t *arr, char *filename)
{
    fptr = fopen(filename, "w");
    for (i = 0; i<28; i++)
    {
	if(arr[i].link != NULL)
	{
	    main_t *mtemp = arr[i].link;
	    while(mtemp != NULL)
	    {
		sub_t *stemp = mtemp->slink;

		fprintf(fptr, "#%d;",i);
		fprintf(fptr, "%s;%d;", mtemp->word, mtemp->filecount);
		while(stemp != NULL)
		{
		    fprintf(fptr, "%s;%d;", stemp->filename, stemp->wordcount);
		    stemp = stemp->sublink;
		}
		fprintf(fptr, "#\n");
		mtemp = mtemp->mlink;
	    }
	}
    }
}


int search_database(hash_t *arr, char *word)
{
    hash_t *temp = &arr[check_index(word[0])];

    main_t *mtemp = temp->link;
    while (mtemp != NULL)
    {
	if (strcmp(mtemp->word, word) == 0)
	{
	    flag = 1;
	    sub_t *stemp = mtemp->slink;
	    while (stemp != NULL)
	    {
		printf("%s found %d times in %s.\n", mtemp->word, stemp->wordcount, stemp->filename);
		stemp = stemp->sublink;
	    }
	    return SUCCESS;
	}
	mtemp = mtemp->mlink;
    }
}

main_t *create_update_mainnode(char *word, int filecount)
{
    main_t *new = malloc(sizeof(main_t));
    if (new == NULL)
    {
	printf("Main node not created.\n");
	return FAILURE;
    }
    else
    {
	new->filecount = filecount;
	strcpy(new->word, word);
	new->mlink = NULL;
	new->slink = NULL;
    }

    return new;
}

sub_t *create_update_subnode(char *filename, int wordcount)
{
    sub_t *new = malloc(sizeof(sub_t));
    if (new == NULL)
    {
	printf("Sub node not created.\n");
	return FAILURE;
    }
    else
    {
	new->wordcount = wordcount;
	strcpy(new->filename, filename);
	new->sublink = NULL;
    }
    return new;
}

int validate_update_file(char *filename)
{
    fptr = fopen(filename, "r");
    char line[100];
    while (fscanf(fptr, "%s", line) == 1)
    {
	if (line[0] == '#' && line[strlen(line)-1] == '#')
	{
	    continue;
	}
	else
	{
	    printf("The file %s is not of needed format.\n", filename);
	    return FAILURE;
	}
    }
}

int update_database(hash_t *arr, char *filename)
{
    if(database_updated)
    {
	return EXIST;
    }

    if (validate_update_file(filename) != FAILURE)
    {
	fptr = fopen(filename,"r");
	if (fptr == NULL)
	{
	    printf("%s cannot be opened.", filename);
	    return FAILURE;
	}

	char str[100];
	char word[50];
	char file[50];
	char filecopy[50];
	int wordcount, filecount, index;

	while(fscanf(fptr,"%s", str) == 1)
	{
	    index = atoi(strtok(str+1, ";"));
	    strcpy(word,strtok(NULL, ";"));
	    filecount = atoi(strtok(NULL, ";"));

	    if(arr[index].link == NULL)
	    {
		//printf("Index Null Mainnode for %s.\n", word);
		arr[index].link = create_update_mainnode(word, filecount);
		if (arr[index].link == NULL)
		{
		    //printf("Mainnode not created for %s.\n", word);
		    return FAILURE;
		}
	    }
	    else
	    {
		//printf("Main for %s created\n", word);
		main_t *mtemp = arr[index].link;
		while (mtemp->mlink != NULL)
		{
		    mtemp = mtemp->mlink;
		}
		mtemp->mlink = create_update_mainnode(word, filecount);
		if (mtemp->mlink == NULL)
		{
		    //printf("Mainode note created for %s.\n", word);
		    return FAILURE;
		}
	    }
	    main_t *mtemp  = arr[index].link;
	    while(mtemp->mlink != NULL)
	    {
		mtemp=mtemp->mlink;
	    }
	    for (i=0; i<filecount; i++)
	    {
		strcpy(file, strtok(NULL, ";"));
		wordcount = atoi(strtok(NULL, ";"));
		strcpy(filecopy, file);

		//printf("%s>", file);
		if (check_duplicate_files(filecopy, &head2) == SUCCESS && check_duplicate_files
			(filecopy, &head) == SUCCESS)
		{
		    //printf("%s added to the list.\n", file);
		    create_file_list(filecopy, &head2);
		}

		if (mtemp->slink == NULL)
		{
		    //printf("Head updated for %s\n", word);
		    mtemp->slink = create_update_subnode(file, wordcount);
		}
		else
		{
		    //printf("Sub updated for %s\n", word);
		    sub_t *stemp = mtemp->slink;
		    while (stemp->sublink != NULL)
		    {
			stemp=stemp->sublink;
		    }
		    stemp->sublink = create_update_subnode(file, wordcount); 
		}
	    }
	}
	database_updated = true;
	printf("\n\n");
	//print_list(head2);
	fclose(fptr);
	return SUCCESS;
    }
}

void display_HT(hash_t *arr, int size)
{
    struct mainnode *temp;
    for(int i = 0; i < size; i++)
    {
	printf("[%d] - %p", i, arr[i].link);
	temp = arr[i].link;
	while(temp)
	{
	    temp = temp->mlink;
	}
	printf("\n");
    }
}


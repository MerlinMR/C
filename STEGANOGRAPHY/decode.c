#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "types.h"
#define MAGIC_STRING "#*"	//DEFINE THE MAGIC STRING TO VALIDATE THE DATA THAT WILL BE DECODED
extern int mg_size;

//	Input: ./a.out -d output.bmp [data]
//		   0    1     2        3     = 4 args

/*<TO VALIDATE THE COMMAND LINE ARGUMENTS>*/
Status read_and_validate_decode_args(int argc, char *argv[],DecodeInfo *decInfo)
{
    if(argc >= 3)
    {
	if(strstr(argv[2],".bmp") != NULL)
	{
	    decInfo -> stego_image_fname = argv[2];
	    printf("Encoded file name: %s\n",decInfo -> stego_image_fname);
	    if(argc == 4)
	    {
		if(strchr(argv[3],'.') != NULL)
		{
		    printf("Enter needed output file (Argument 4) name without extension\n");
		    return d_failure;
		}
		else
		{
		    decInfo -> secret_fname = argv[3];
		    strcpy(decInfo -> secret_fname,argv[3]);
		    printf("Secret file name: %s\n",decInfo -> secret_fname);
		    return d_success;
		}
	    }
	    else
	    {
		argv[3] = "data";
		decInfo -> secret_fname = argv[3];
		printf("Secret file name assigned as %s\n",decInfo -> secret_fname);
		return d_success;
	    }
	}
	else
	{
	    printf("Enter valid input file\n");
	    return d_failure;
	}
    }
    else
    {
	printf("Invalid number of arguments\n");
	return d_failure;
    }
    return d_success;

}

/*<TO PERFORM ENCODING>*/
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_file(decInfo) == d_success)							//OPEN FILES
    {
	printf("Files opened\n");
	if(decode_magic_string(decInfo->magic_string,decInfo) == d_success)		//TO DECODE THE MAGIC STRING
	{
	    printf("Magic string decoded\n");
	    if(decode_secret_file_extn_size(decInfo) == d_success)			//DECODE THE SECRET FILE EXTENSION SIZE
	    {
		printf("Secret file extension size decoded\n");
		if(decode_secret_file_extn(decInfo -> file_extn,decInfo) == d_success)	//DECODE SECRET FILE EXTENSION
		{
		    printf("Secret file extension decoded\n");
		    if(decode_secret_file_size(decInfo->secret_file_size,decInfo) == d_success)	//DECODE SECRET FIE SIZE
		    {
			printf("Secret file size decoded\n");
			if(decode_secret_file_data(decInfo ->secret_data,decInfo) == d_success)
			{
			    printf("Secret file data decoded\n");
			    return d_success;
			}
			else
			{
			    printf("Secret file data decoded\n");
			    return d_failure;
			}
		    }
		    else
		    {
			printf("Secret file size decoded\n");
			return d_failure;
		    }
		}
		else
		{
		    printf("ERROR: Secret file extension not decoded\n");
		    return d_failure;
		}
	    }
	    else
	    {
		printf("ERROR: Secret file extension size not decoded\n");
		return d_failure;
	    }
	}
	else
	{
	    printf("ERROR: Magic string is not valid. Try again.\n");
	    return d_failure;
	}
    }
    else
    {
	printf("ERROR: Files not opened\n");
	return d_failure;
    }
}

/*<TO OPEN FILES>*/
Status open_file(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname,"r");	//OPEN THE SRC(ENCODED) IMAGE
    fseek(decInfo -> fptr_stego_image,54,SEEK_SET);				//SKIP 54 BYTES AS IT IS HEADER
    if(decInfo -> fptr_stego_image == NULL)					//IF FILES NOT OPENED RETURN ERROR
    {
	perror("fopen");
	fprintf(stderr,"ERROR: Unable to open the file %s\n",decInfo -> stego_image_fname);
	return d_failure;
    }

    decInfo -> magic_string = (char *)malloc(strlen(MAGIC_STRING)+1);		//ASSIGN MEMORY FOR magic_string TO STORE THE data to be decoded
    return d_success;
}

/*<TO DECODE MAGIC STRING>*/
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{

    char buffer[8];
    char ch;
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
	fread(buffer,8,1,decInfo ->fptr_stego_image);		//TAKES 8 BYTES FROM THE STEGO IMAGE EACH TIME AND STORE IT IN A BUFFER
	ch = decode_byte_from_lsb(&magic_string[i],buffer);	//THE BUFFER IS THEN PASSED TO PERFORM DECODING
	magic_string[i] = ch;					//THE DECODED DATA IS STORED AT ASSIGNED MEMORY OF magic_string
	//printf("%s", magic_str);
    }

    printf("Decoded Magic string is %s\n",decInfo -> magic_string);
    if(strcmp(MAGIC_STRING,decInfo->magic_string)==0)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}

/*<TO DECODE SIZE OF SECRET FILE EXTENTION>*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];	//CREATE A BUFFER OF SIZE 32
    int size = 0;
    fread(buffer,32,1,decInfo -> fptr_stego_image);	//READ 32 BYTES FROM THE SRC IMAGE ANDSTORE IT IN BUFFER
    decode_size_from_lsb(buffer,&size);			//PASS THE BUFFER TO PERFORM DECODING
    decInfo -> file_extn_size = size;			//DATA FROM PREVIOUS STEP IS STORED AS file_extn_size
    decInfo -> file_extn =(char *)malloc(decInfo -> file_extn_size + 1);	//ASSIGN MEMORY MANUALLY TO file_extn TO STORE DATA TO BE DECODED
    printf("File extension size is %d\n",decInfo -> file_extn_size);
    return d_success;
}

/*<TO DECODE SIZE FROM LSB>*/
Status decode_size_from_lsb(char *buffer,int *size)
{
    for(int i=0;i<32;i++)
    {
	*size = ((buffer[i] & 0x01 ) << (31-i)) | *size;
    }
    printf("Size = %d\n",*size);
    return *size;
}

/*<TO DECODE BYTE FROM LSB>*/
char decode_byte_from_lsb(char *data,char *buffer)
{
    unsigned char ch = 0x00;
    for(int i=0;i<8;i++)
    {
	ch = (( buffer[i] & 0x01 ) << (7-i)) | ch;
    }
    return ch;
}

static char str2[30];
/*<TO DECODE SECRET FILE EXTENSION>*/
Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo)
{
    char str[8];  

    for(int i=0; i<decInfo->file_extn_size; i++)
    {
	fread(str,8,1,decInfo ->fptr_stego_image);	//READ 8 BYTES FROM fptr_stego_image TO STR
	char ch = decode_byte_from_lsb(&file_extn[i],str);	//PASS IT TO DECODE AND STORE OUTPUT IN ch
	file_extn[i] = ch;	//STORE THE OBTAINED ch in ASSIGNED MEMORY OF file_extn
    }
    printf("File extn is %s\n",decInfo -> file_extn);

    char str1[8];
    strcpy(str1,decInfo -> file_extn);
    strcpy(str2,decInfo -> secret_fname);
    strcat(str2,str1);


    printf("Secret file is %s\n",str2);

    return d_success;
}

/*<TO DECODE SECRET FILE SIZE>*/
Status decode_secret_file_size(int secret_file_size,DecodeInfo *decInfo)
{
    char image_buffer[32];
    int size=0;
    fread(image_buffer,32,1,decInfo -> fptr_stego_image);

    decode_size_from_lsb(image_buffer,&size);
    decInfo -> secret_file_size = size;
    decInfo -> secret_data =  (char *)malloc(decInfo -> secret_file_size+1);

    printf("Size of secret file is %d\n",decInfo ->secret_file_size);
    return d_success;
}

/*<TO DECODE SECRET FILE DATA>*/
Status decode_secret_file_data(char *secret_data,DecodeInfo *decInfo)
{
    char str[8];
    int i;
    decInfo -> fptr_secret = fopen(str2,"w");
    if (decInfo -> fptr_secret == NULL)
    {
	fprintf(stderr,"Unable to open the file\n");
	return d_failure;
    }
    else
    {
	for (i = 0; i<decInfo -> secret_file_size; i++ )
	{
	    fread(str,8,1,decInfo-> fptr_stego_image);
	    char ch = decode_byte_from_lsb(&secret_data[i],str);
	    fwrite(&ch,1,1,decInfo -> fptr_secret);
	    secret_data[i] = ch;
	}
	return d_success;
    }
}


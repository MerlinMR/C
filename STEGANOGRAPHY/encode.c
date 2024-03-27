#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include <string.h>
extern int mg_size;
//extern char magic_str;

/* Function Definitions */

//	Input: ./a.out -e beautiful.bmp secret.txt [output.bmp]
//		   0	1	2	    3		4	= 5 arguments

/*<TO VALIDATE THE COMMAND LINE ARGUMENTS>*/

Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    if (argc >= 4)
    {
	if(strstr(argv[2],".bmp") != NULL)			//VALIDATE THE SECOND ARGUMENT WITH '.bmp'
	{
	    encInfo->src_image_fname=argv[2];			//IF PASSED STORE THE ARGUMENT AS src_image_fname
	    printf("Source image name obtained.\n");
	    if(strchr(argv[3], '.') != NULL)			//VALIDATE THE THIRD ARGUMENT WITH '.'
	    {							//IF PASSED,
		char *extn;					//CREATE A CHAR extn TO STORE ADDRESS
		extn = strchr(argv[3], '.');			//STORE THE ADDRESS WHERE '.' IS PRESENT IN THE FILE NAME
		strcpy(encInfo->extn_secret_file,extn);		//STORE THE EXTENSION OBTAINED IN PREVIOUS STEP IN extn_secret_file 
		encInfo->secret_fname=argv[3];			//STORE THE THIRD ARGUMENT AS secret_fname

		if(argv[4] != NULL)				//CHECK IF 4TH ARGUMENT IS PRESENT OR NOT
		{
		    if(strstr(argv[4],".bmp") != NULL)		//IF PRESENT VALIDATE WITH '.bmp'
		    {
			encInfo -> stego_image_fname = argv[4];	//IF PASSED STORE IT AS stego_image_fname
			return e_success;
		    }
		    else
		    {
			printf("ERROR : Pass output file with .bmp extension\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Default name assigned\n");
		    encInfo->stego_image_fname = "output.bmp";	//IF NOT PRESENT, ASSIGN A DEFAULT VALUE
		    return e_success;
		}
	    }
	    else
	    {
		printf("ERROR: Pass 4th argument in format name.[extension]\n");
		return e_failure;
	    }
	}
	else

	{
	    printf("ERROR: Pass 3rd argument with .bmp extension\n");
	    return e_failure;
	}
    }
    else
    {
	printf("ERROR: Pass atleast 4 arguments.\n");
	return e_failure;
    }
}

/*<TO PERFORM ENCODING>*/

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
	printf("Files opened\n");
	if(check_capacity(encInfo)==e_success)
	{
	    printf("Capacity checked\n");
	    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	    {
		printf("BMP Header copied\n");
		if(encode_magic_string(encInfo -> magic_str,encInfo) == e_success)
		{
		    printf("Magic String encoded\n");
		    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_success)
		    {
			printf("Secret file extension size encoded\n");
			if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
			{
			    printf("Secret file Extension encoded\n");
			    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
			    {
				printf("Secret file size encoded\n");
				if(encode_secret_file_data(encInfo) == e_success)
				{
				    printf("Secret file data encoded\n");
				    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
				    {
					printf("Remaining data copied\n");
					return e_success;
				    }
				    else
				    {
					printf("ERROR: Remaining data not copied\n");
					return e_failure;
				    }
				}
				else
				{
				    printf("ERROR: Secret file data not encoded\n");
				    return e_failure;
				}
			    }
			    else
			    {
				printf("ERROR: Secret file size not encoded\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("ERROR: Secret file extension not encoded\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("ERROR: Secret file extension size not encoded\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("ERROR: Magic string not encoded\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("ERROR: Header not copied\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("EEROR: Capacity not checked\n");
	    return e_failure;
	}

    }
    else
    {
	printf("ERROR: File not opened\n");
	return e_failure;
    }

}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
*/

/*<TO OPEN SRC, SECRET AND ENCODED(STEGO/OUTPUT) FILE>*/

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");	//OPEN THE FILE STORED AT src_image_fname
    if (encInfo->fptr_src_image == NULL)	//IF IT DOES NOT OPEN, RETURN ERROR
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");		//OPEN THE FILE STORED AT secret_fname
    if (encInfo->fptr_secret == NULL)		//IF IT DOES NOT OPEN, RETURN ERROR
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
	return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");	//OPEN THE OUTPUT/STEGO FILE STORED AT stego_image_fname
    if (encInfo->fptr_stego_image == NULL)	//IF IT DOES NOT OPEN, RETURN ERROR
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
	return e_failure;
    }

    return e_success;	//IF ALL FILES OPENED SUCCESSFULLY THEN RETURN e_success
}

/*<TO CHECK IF THE SOURCE IMAGE CAN EFFICENTLY ENCODE/HIDE ALL ELEMENTS LIKE MAGIC STRING, SECRET FILE SIZE ETCETERA>*/

Status check_capacity(EncodeInfo *encInfo)
{
    printf("Enter the magic string to be encoded : ");
    scanf("%s",encInfo->magic_str);	//STORE THE ENTERED STRING IN magic_str
    mg_size = strlen(encInfo->magic_str);
    
    printf("Magic string length is %d\n", mg_size);
    encInfo->image_capacity = get_file_size(encInfo->fptr_src_image);	//GET FILE SIZE OF THE SOURCE IMAGE AND STORE IT IN image_capacity 
    rewind(encInfo->fptr_src_image);	//REWIND TO SET FILE POINTER AT O
					//printf("Size of source file is %d\n",encInfo->image_capacity);


    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);	//GET FILE SIZE OF SECRET FILE AND STORE IT IN size_secret_file
    rewind(encInfo->fptr_secret);	//REWIND TO SET THE FILE POINTER AT 0
					//printf("Size of secret file is %ld\n",encInfo->size_secret_file);

					//The size of bits to be encoded must be lesser than image capacity (total bytes of source image)

    int data=(strlen(encInfo->magic_str)+sizeof(int)+strlen(encInfo->extn_secret_file)+sizeof(int)+encInfo->size_secret_file)*8;	//sizeof(int) for extn size & file size

    if (data<encInfo->image_capacity)
    {
	return e_success;
    }
    else
    {
	printf("ERROR: Source image size is lesser than size of data to be stored\n");
	return e_failure;
    }
}

/*<TO COPY FIRST 54 BYTES OF THE SRC IMAGE. THESE BYTES SHOULD BE MAINTAINED SAME AS IT CONTAINS DETAILS RELATED TO THE IMAGE>*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];				//CREATE A BUFFER OF SIZE 54
    fread(buffer,54,1,fptr_src_image);		//READ/COPY THE 54 BYTES INTO THE BUFFER ONE AT A TIME
    fwrite(buffer,54,1,fptr_dest_image);	//WRITE/PASTE THE CONTENTS IN BUFFER TO THE DEST IMAGE
    return e_success;
}

/*<TO ENCODE MAGIC STRING>*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)	//ENCODE DATA IN MAGIC STRING
    {
        printf("Magic string encoded to image\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Magic string not encoded\n");
        return e_failure;
    }
}

/*<TO ENCODE SIZE INTO THE LSB>*/
Status encode_size_to_lsb(char *buffer,long ext_size, EncodeInfo *encInfo)
{
    for(int i=0;i<32;i++)	//RUN LOOP FOR 32 TIMES FOR 32 BITS OF THE INTEGER
    {
        buffer[i]=buffer[i]&0xfe | (unsigned)(ext_size&(1<<(31-i)))>>(31-i);
    }
    return e_success;
}

/*<TO ENCODE SECRET FILE EXTENSION SIZE>*/
Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];	//CREATE A BUFFER OF SIZE 32
    fread(buffer,1,32,encInfo->fptr_src_image);

    if(encode_size_to_lsb(buffer,file_size,encInfo)==e_success)	//ENCODE 32 BITS TO LSB OF 32 BYTES
    {
        fwrite(buffer,1,32,encInfo->fptr_stego_image);	//WRITE CONTENTS OF BUFFER INTP THE DET IMAGE
        //printf("Secret file extension size: %ld\n",file_size);
        printf("Secret file extension encoded\n");
        return e_success;	//IF SUCCESS FUNCTION WILL BE EXITED HERE
    }
    printf("Secret file extension size not encoded\n");
    return e_failure;
}

/*<TO ENCODE SECRET FILE EXTENSION>*/
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    //TO ENCODE THE EXTENSION TO LSB OF SRC IMAGE AND STORE IT IN DEST (Eg: .txt)
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("Secret file extension encoded\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Secret file extension not encoded\n");
        return e_failure;
    }
}

/*<TO ENCODE SECRET FILE SIZE>*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];	//TAKE BUFFER OF SIZE32 TO ENCODE INTEGER(32 BIT)
    //printf("secret file size: %ld\n",file_size);
    fread(buffer,1,32,encInfo->fptr_src_image);	//READ 32 BIT FROM SRC IMAGE TO BUFFER IMAGE
    
    if(encode_size_to_lsb(buffer,file_size,encInfo)==e_success)
    {
        fwrite(buffer,1,32,encInfo->fptr_stego_image);	//WRITE THEN CONTENTS IN BUFFER TO DETN IMAGE
        //printf("encode_secret_file_size successfully\n");
        return e_success;	//IF SUCCESS FUNCTION WILL BE EXITED FROM HERE
    }
    printf("Secret file size not encoded\n");
    return e_failure;
}

/*<TO ENCODE SECRET FILE DATA>*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buff_data_secret[encInfo->size_secret_file+1];	//CREATE A BUFFER OF SIZE SECRET FILE DATA
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    fread(buff_data_secret, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    char str[8];
    for (int i=0; i<encInfo->size_secret_file; i++)
    {
	fread(str, 8, 1, encInfo -> fptr_src_image);
	encode_byte_to_lsb(buff_data_secret[i],str);
	fwrite(str, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
*/

/*<TO GET IMAGE SIZE FOR BMP>*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);
    rewind(fptr_image);

    // Return image capacity
    return width * height * 3;
}

/*<TO ENCODE DATA TO IMAGE>*/
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];	//TAKE A BUFFER OF SIZE 8 (IN DATA, A CHARACTER HOLD 8 BITS)
    //printf("size of %s = %d\n", data,size);
    for(int i=0;i<size;i++)
    {
	fread(image_buffer,1,8,fptr_src_image);

	if(encode_byte_to_lsb(data[i],image_buffer)==e_success)	//ENCODE THEN BITS IN BUFFER AND WRITE IT INTO DESTN IMAGE 
	{
	    fwrite(image_buffer,1,8,fptr_stego_image);
	}
    }
    return e_success;
}

/*<TO ENCODE BYTE TO LSB>*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)	//RUN LOOP FOR 8 TIMES TO TAKE EVERY BIT FROM CHARACTER
    {
	image_buffer[i]=(image_buffer[i]&0xfe) | (unsigned)(data&(1<<(7-i)))>>(7-i);
    }
    return e_success;
}

/*<TO COPY REMAINING DATA FRM SRC TO DEST IMAGR>*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0)
    {
	fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

/*<TO GET FILE SIZE>*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);	//SEEK_END SETS FILE POINTER TO LAST THUS RETURNING TOTAL SIZE OF THE FILE
    return ftell(fptr);
}


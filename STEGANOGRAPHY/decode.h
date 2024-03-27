#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char *magic_string;

    /* secret file Info */
    char *secret_fname;
    FILE *fptr_secret;
    int file_extn_size;
    char *file_extn;
    char *secret_data;
    int secret_file_size;
} DecodeInfo;


/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p files */
Status open_file(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(int secret_file_size,DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(char *secret_data,DecodeInfo *decInfo);

/* To decode the size from the lsb position in each byte */
Status decode_size_from_lsb(char *buffer,int *size);

/* To decode the each byte from the lsb position */
char decode_byte_from_lsb(char *data,char *buffer);




#endif


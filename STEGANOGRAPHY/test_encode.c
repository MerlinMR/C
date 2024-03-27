#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int mg_size;
int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    int ret;

    ret=check_operation_type(argv);
    if(ret == e_encode)
    {
	if(read_and_validate_encode_args(argc, argv,&encInfo) == e_success)
	{
	    printf("Encode Validation Successful\n");
	    if (do_encoding(&encInfo) == e_success)
	    {
		printf("\n<ENCODING COMPLETED>\n");
	    }
	    else
	    {
		printf("ERROR: Not encoded\n");
	    }
	}
    }
    else if (ret == e_decode)
    {
	if(read_and_validate_decode_args(argc,argv,&decInfo) == d_success)
	{
	    printf("Decode Validation Successful\n");
	    if (do_decoding(&decInfo)==d_success)
	    {
		printf("\n<DECODING COMPLETED>\n");
		return 0;
	    }
	    else
	    {
		printf("ERROR: Not Decoded\n");
		return 0;
	    }
	}
    }
    else
    {
	printf("No operation selected.\n");
    }
}



OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
	//printf("e_encode()\n");
	return e_encode;
    }
    else if(strcmp(argv[1],"-d") == 0)
    {
	//printf("e_decode\n");
	return e_decode;
    }
    else
    {
	//printf("Neither Encode nor Decode\n");
	return e_unsupported;
    }
}


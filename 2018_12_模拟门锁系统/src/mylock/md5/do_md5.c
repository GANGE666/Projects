#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.c"
 
int do_md5(char * encrypt, char *decrypt)
{
	int i;
   
	MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt);    
    
	printf("����ǰ:%s\n���ܺ�:",encrypt);
	for(i=0;i<16;i++)
	{
		printf("%02x",decrypt[i]);
	}
	printf("\n");	
	return 0;
}
#include<stdio.h>
#include<string.h>
#include"keyfilemanager.c"

#define READ_SIZE 16

int main(){
	unsigned char key[READ_SIZE];
	int i = 0;
	
	memset(key, 0, sizeof(key));
	
	if(readKeyFile(key) < 0)
		printf("test.c\tmain()\t******************readKeyFile Fail*************\n");
	
	
	for(i=0;i<READ_SIZE;i++)
	{
		printf("%02x",key[i]);
	}
	printf("\n");
	
	for(i=0; i<READ_SIZE;i++)
		key[i] += 1;
		
	if(writeKeyFile(key) < 0)
		printf("test.c\tmain()\t******************writeKeyFile Fail*************\n");
	
	return 0;
}

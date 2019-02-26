#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define READ_SIZE 16
#define DEBUG

//#define STORED_KEY_FILE "./keyfile"

static int fd;

//初始化，打开储存key_hash的文件
int cmp_init(void)
{	
	/*
	fd = open(STORED_KEY_FILE, O_RDONLY | O_NONBLOCK);
	if(fd < 0)
	{	
		printf("No stored key file!\n");
		return -1;
	}
	else
	{
		printf("Stored key file opened!\n");
	}
	*/
	return 0;		
}


//将输入密码hash与存储的hash作比较
int cmp_main(char * input_hash)
{
	unsigned char key[READ_SIZE];
	int i = 0;
	
	memset(key, 0, sizeof(key));
	
	if(readKeyFile(key) < 0)
	{
		printf("test.c\tmain()\t******************readKeyFile Fail*************\n");
		return -1;
	}
	
#ifdef DEBUG
	//DEBUG 显示hash后的字符串
	printf("input_hash:");
	for(i=0;i<16;i++)
		printf("%02x",input_hash[i]);
	printf("\n");
	printf("stored_key:");
	for(i=0;i<16;i++)
		printf("%02x",key[i]);
	printf("\n");
#endif	
	
	//比较两组hash值
	for(i=0;i<READ_SIZE;i++){
		if(input_hash[i] != key[i]) 
		{
			//不相等则擦除hash并返回错误码
			printf("******************uncomp!!!!!*********************\n");
			
			memset(input_hash, 0, sizeof(input_hash));
			memset(key, 0, sizeof(key));
			
			return -1;
		}
	}
	if(i == 16)
	{
		//相等则返回
		printf("input_hash and key cmp!\n");
		memset(key, 0, sizeof(key));
		return 1;
	}
	
	
	return 1;
}







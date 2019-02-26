#include<string.h>
#include<stdio.h>
#include<fcntl.h>

#define READ_SIZE 16+1

#define XOR_KEY 0xAA


//加解密从文件中读取到的数值 
void decryKeyFileStr(unsigned char * keyfiledata, unsigned char * key){
	int i = 0;
	for(; i < READ_SIZE; i++)
		key[i] = keyfiledata[i] ^ XOR_KEY;
		
	return;	
}

//读取文件并解密 
int readKeyFile(char * f_path, unsigned char * key){
	int i = 0;
	
	//读取到未解密的key_data
	unsigned char keyfiledata[READ_SIZE];
	//读取文件是否成功的标志变量 
	int readfileflag = 0;
	
	//打开文件，常规操作 
	int fd;

	printf("enter readKeyFile!\n");


	fd = open(f_path, O_RDONLY);
	if(fd < 0)
	{
		printf("keyfilemanager.c\treadKeyFile()\t*************************KEY FILE OPEN FAIL!*****************************\n");
		return -1;
	}
	
	//读取文件，并检测是否出错 
	readfileflag = read(fd, keyfiledata, READ_SIZE);
	if(readfileflag <= 0){
		printf("keyfilemanager.c\treadKeyFile()\t*************************KEY FILE READ ERROR!*****************************\n");
		return -1;
	}
	close(fd);
	
#ifdef DEBUG
	printf("keyfilemanager.c\treadKeyFile()\t")	;
	for(i=0;i<READ_SIZE;i++)
	{
		printf("%02x",keyfiledata[i]);
	}
	printf("\n");
#endif	
	
	//解密读取到的字符串，并copy至传入的地址中，准备传出 
	decryKeyFileStr(keyfiledata, key);	
	 
	//清除局部变量 
	for(i = 0; i < READ_SIZE; i++)
		keyfiledata[i] = 0;
	fd = 0;
	
	return 0;
}


int writeKeyFile(char * f_path, unsigned char * key){
	int i = 0;
	
	//解密后的的key_data
	unsigned char keyfiledata[READ_SIZE];
	//写文件是否成功的标志变量 
	int readfileflag = 0;
	
	//打开文件，常规操作 
	int fd;
	fd = open(f_path, O_WRONLY);
	if(fd < 0)
	{
		printf("keyfilemanager.c\twriteKeyFile()\t*************************KEY FILE OPEN FAIL!*****************************\n");
		return -1;
	}
	
	//加密传入的的字符串，并copy至要写入的地址中，准备写入 
	decryKeyFileStr(key, keyfiledata);
	
	//写文件，并检测是否出错 
	readfileflag = write(fd, keyfiledata, READ_SIZE);
	if(readfileflag <= 0){
		printf("keyfilemanager.c\twriteKeyFile()\t*************************KEY FILE WRITE ERROR!*****************************\n");
		return -1;
	}
	close(fd);
	
#ifdef DEBUG
	printf("keyfilemanager.c\twriteKeyFile()\t")	;
	for(i=0;i<READ_SIZE;i++)
	{
		printf("%02x",keyfiledata[i]);
	}
	printf("\n");
#endif		
	 
	//清除局部变量 
	for(i = 0; i < READ_SIZE; i++)
		keyfiledata[i] = 0;
	fd = 0;
	
	return 0;
}






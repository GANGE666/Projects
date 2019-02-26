/************************************************
*	by zou jian guo<zounix@126.com>	
*		2004.9.27 14:30			
*						
*   the driver is s3c2410_da_max504.c in drivers/char
*************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "keyboard.h"
#include "../md5/md5.h"

#define DEBUG

int encrypt(char * key, char * hash);

/****************************************************************/
//int main()
int getpdhash(char * hash_key, char * hash_key_bak)
{
	char mykey = 0;
	//两组input序列，用于防止传输过程中的错误，限长10位
	char input_key[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char input_key_bak[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	//用于储存hash加密后的密码
	//char hash_key[65] = {};
	//char hash_key_bak[65] = {};
	int i = 0;
	//输入字符串的压码
	int sum = 0;
	
	//键盘初始化
	kbd_init();
	
	//当输入的键不为回车时，持续输入
	while(mykey!=13)
	{   
		sum += mykey;
		
		mykey=get_key();
        //printf("which key you press is %c  \n", mykey,mykey,mykey);
		printf("%d\tkey down\n", i);
		
		input_key[i%10] = mykey;
		input_key_bak[i%10] = mykey;
		i = (i+1)%10;
	}
	
	//用压码替换回车的asc2
	input_key[i] = sum;
	input_key_bak[i] = sum;
	sum = 0;
	
	
	//将回车后覆盖的键清除
	for(; i<10; i++)
	{
		input_key[i] = 0;
		input_key_bak[i] = 0;
	}
	
#ifdef DEBUG
	//DEBUG 显示输入的字符串
	for(i = 0; i < 10; i++)
		printf("%d\t%c\n", i, input_key[i]);
#endif
	
	//对输入密码进行hash
	encrypt(input_key, hash_key);
	encrypt(input_key_bak, hash_key_bak);
	
	//对密码进行hash后清除保存原密码的数组
	for(i = 0; i < 10; i++)
	{	
		input_key[i] = 0;
		input_key_bak[i] = 0;
	}
	
	//关闭键盘
	kbd_close();
	
#ifdef DEBUG
	//DEBUG 显示hash后的字符串
	printf("hash_key:");
	for(i=0;i<16;i++)
		printf("%02x",hash_key[i]);
	printf("\n");
	printf("hash_key_bak:\t%s\n", hash_key_bak);
#endif	
	
#if 0
	//test 不同文件间的函数调用
	printf("test return: %d\n", test(hash_key));
	printf("test_ed_hash_key: %s\n", hash_key);
#endif	
	
	//比较两组hash值
	if(strcmp(hash_key, hash_key_bak) == 0)
	{
		//相等则返回
		printf("hash_key and hash_key_bak cmp!\n");
		return 0;
	}
	else
	{
		//不相等则擦除hash并返回错误码
		printf("******************uncomp!!!!!*********************\n");
		
		memset(hash_key, 0, sizeof(hash_key));
		memset(hash_key_bak, 0, sizeof(hash_key_bak));
		
		return -1;
	}

	
	return 0;
}

//对输入密码进行hash
int encrypt(char * key, char * hash)
{
	int i = 0;
	MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,key,strlen((char *)key));
	MD5Final(&md5,hash);
	/*
	for(; i < 64; i++)
		hash[i] = key[i%10];
	*/
	return 1;
}

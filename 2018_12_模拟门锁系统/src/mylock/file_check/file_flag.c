#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../mylock_config.h"

int set_flag(){
	char buf[10];
	int i = 0;
	
	memset(buf, 0, sizeof(buf));
	
	for(i=0;i<10;i++)
		buf[i] = 1;
	
	file_cmp_write(FILE_FLAG_PATH, buf);
	
	return 0;
}

int remove_flag(){
	char buf[10];
	int i = 0;
	
	memset(buf, 0, sizeof(buf));
	
	for(i=0;i<10;i++)
		buf[i] = 0;
	
	file_cmp_write(FILE_FLAG_PATH, buf);
	
	return 0;
}

//检查标志文件，返回1表示seted， 0表示removed
int check_flag(){
	char buf[10];
	int i = 0, sum = 0;;
	
	memset(buf, 0, sizeof(buf));
	
	file_cmp_read(FILE_FLAG_PATH, buf);
	
	sum = 0;
	
	for(i=0;i<10;i++)
		sum += buf[i];
	
	if(sum > 5)
		return 1;
	
	return 0;
}


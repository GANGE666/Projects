#include<stdio.h>
#include<unistd.h>
#include "mylock_config.h"

extern * KEY_FILE_PATH[];

int hadel_unremove_file_flag(){
	//char * KEY_FILE_PATH[] = {"./keyfile/keyfile1", "./keyfile/keyfile2", "./keyfile/keyfile3"};
	if(file_cmp(KEY_FILE_PATH, KEY_FILE_NUM) == -1){
		printf("[ERROR]\tKEYFILE BROKEN!\n");
		return -1;
	}
	
	remove_flag();
	
	return 0;
}


int mylock_init(){
  char buf[80];
  printf("init begin\n");
	if(kbd_init() != 0){
		printf("[ERROR]\tInit Fail!\n");
		exit(0);
	}
	

	getcwd(buf,sizeof(buf));
	printf("path: %s\n",buf);

	//功率随机初始化
	random_init();
	
	//检查标志
	if(check_flag() == 1)
		if(hadel_unremove_file_flag() == -1)
			error_handel();
	
	printf("[INFO]\tInitialization Complete!\n");
	return 0;
}

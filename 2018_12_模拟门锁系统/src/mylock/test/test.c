#include<stdio.h>
#include<string.h>
#include "../mylock_config.h"


#define DEBUG

//模式变量默认1为开启，0为关闭
int mode_sleep = 0;
int mode_changepwd = 0;


//计数器
int count_sleep = 0;
int count_changepwd = 0;
int count_changepwdstate = 0;

//输入数组
char input_key[KEY_LEN+2] = {0};
int count_inputkey = 0;
int check_inputkey_num = 0;

//经过salt hash的输入数组
char hash[HASH_LEN] = {0};

//检查结果标志
int check_flag = 0;

//更改密码数组
char newhash[HASH_LEN] = {0};


void clean_all(){
	memset(input_key, 0, sizeof(input_key));
	memset(hash, 0, sizeof(hash));
	memset(newhash, 0, sizeof(newhash));	
}

int main(){
	//临时变量
	char ch;
	int sum1 = 0, sum2 = 0, tmp, debug;
	long long time = 0;
/*	
	tmp = checkpwd(hash, &sum1, &sum2);
#ifdef DEBUG
	printf("[DEBUG]\tsum1: %d, sum2: %d, res: %d\n", sum1, sum2, tmp);
#endif				
*/	
	test_filecmp();
	
	
	
	return 0;
}


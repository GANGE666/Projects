#include<stdio.h>
#include<string.h>
#include "../mylock_config.h"


#define DEBUG

//ģʽ����Ĭ��1Ϊ������0Ϊ�ر�
int mode_sleep = 0;
int mode_changepwd = 0;


//������
int count_sleep = 0;
int count_changepwd = 0;
int count_changepwdstate = 0;

//��������
char input_key[KEY_LEN+2] = {0};
int count_inputkey = 0;
int check_inputkey_num = 0;

//����salt hash����������
char hash[HASH_LEN] = {0};

//�������־
int check_flag = 0;

//������������
char newhash[HASH_LEN] = {0};


void clean_all(){
	memset(input_key, 0, sizeof(input_key));
	memset(hash, 0, sizeof(hash));
	memset(newhash, 0, sizeof(newhash));	
}

int main(){
	//��ʱ����
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


#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mylock_config.h"
#include "state_check/state_check.h"

#define DEBUG

char tmp[HASH_LEN] = {0};
//char * KEY_FILE_PATH[] = {"/mnt/nfs/exp/mylock_v2_test/mylock/keyfile/keyfile1", "keyfile/keyfile2", "keyfile/keyfile3"};

char * KEY_FILE_PATH[] = {"mylock/keyfile/keyfile1", "mylock/keyfile/keyfile2", "mylock/keyfile/keyfile3"};

extern int current_state;
extern int current_check_psw_fail;

//��ȡ��ǰʱ�䣬��ȷ������
long long getCurrentTime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//��������
int clean_array(char * array)
{
	memset(array, 0, sizeof(array));
	return 0;
}

//�������
void error_handel(){
	
	printf("[WARING]\tStop feed watchdog now!\n");
	
	//ͨ����ѭ�� ��ι���Ź�ʵ������
	while(1);
	
	return;
}

//���»س�����������н��е�һ������
int handel_inputkey(char * inputkey, int len){
	int i = 0;
	inputkey[len] = 0;
	for(i=0;i<KEY_LEN;i++)
		inputkey[len+i] = 0;
	
	len = 0;
	return;
}

int check_inputkey(char * inputkey){
	int i = 0, debug;
	int sum = 0;			
	
	while(inputkey[i+1] != 0){
		sum = (sum + inputkey[i]) & 0xFF;
		i++;
	}		

	if(sum == inputkey[i]){
		i = 0;
		sum = 0;
		return 0;
	}
	else{
		i = 0;
		sum = 0;
		return -1;
	}
		
}

//����hash
int add_salt_hash(char * inputkey, char * hash, int * state){
	int debug, i, checkcode;

#ifdef STATE_CHECK_OPEN	
	//״̬���
	check_update_state(&current_state, REQ_STATE_HASH_ED, STATE_HASH_ED);

	state[1] = 1;
	state[0]++;
#endif
	
	if(check_inputkey(inputkey) == -1)
		return -1;
	
	//strcpy(tmp, SALT);
	//strcat(tmp, inputkey);
	
	strcpy(tmp, inputkey);
	strcat(tmp, SALT);
	
#ifdef DEBUG
	printf("[INFO]\tTMP_ARRAY: ");
	for(debug = 0; debug < HASH_LEN; debug++)
		printf("%2x", tmp[debug]);
	printf("\n");
#endif	
	
	//�������
	random_clac();
	
	do_md5(tmp, hash);
	
	random_clac();
	
	memset(tmp, 0, sizeof(tmp));
	
	#ifdef KEYFILE_CHECK_CODE

	checkcode = 0;
	i = 0;

	while(i<16){
		checkcode += hash[i];
		i ++;
	}

	hash[i] = checkcode & 0xFF;
	
	#endif

	return 0;
}

//У������
int checkpwd(char * hash, int * sum1, int * sum2, int * state){
	int i = 0, debug;
	char file_key[MAX_FILE_SIZE];
	memset(file_key, 0, sizeof(file_key));

#ifdef STATE_CHECK_OPEN	
	//״̬���
	check_update_state(&current_state, REQ_STATE_CHECK_PSW_ED, STATE_CHECK_PSW_ED);
	if(state[0] == 1 && state[1] == 1){
		state[2] = 2;
		state[0] = 2;
	}
	else{
		return -1;
	}
#endif	
	
	//ʹ�人�������󣬷���������ɵ���ʧ��С
	*sum1 = -1;
	*sum2 = -1;

	//�Ƿ��Ѵﵽ�Ƚ�����ʧ�ܵ�����
	if(current_check_psw_fail >= MAX_CHECK_PSW_FAIL)
		return 0;
	
	
	//�������ļ����жԱ�
	if(file_cmp(KEY_FILE_PATH, KEY_FILE_NUM) == -1){
		printf("[ERROR]\tKEYFILE BROKEN!\n");
		return -1;
	}
	
#ifdef DEBUG
	printf("[INFO]\tReading KeyFile!\n");
#endif	
	
	//�������ļ�
	readKeyFile(KEY_FILE_PATH[0], file_key);
	
#ifdef DEBUG
	printf("[INFO]\tRead KeyFile Succ!");
	for(debug = 0; debug < HASH_LEN+5; debug++)
		printf("%2x", file_key[debug]);
	printf("\n");
#endif	
	
	//�������Ա�hash
	while(i < MAX_FILE_SIZE){
		if(hash[i] == 0 || file_key[i] == 0)
			break;
		*sum1 += hash[i] ^ file_key[i];
		*sum2 += hash[i] ^ file_key[i];
		
		//�������
		random_clac();
		
		i++;
	}
	
	*sum1 += 1;
	*sum2 += 1;

#ifdef DEBUG
	printf("[INFO]\tCompare Over! sum1: %d, sum2: %d, hash[i]: %2x, file_key[i]: %2x\n", *sum1, *sum2, hash[i], file_key[i]);
#endif	
	
	if(hash[i] != 0 || file_key[i] != 0)
		return -1;
	
	if(*sum1 == *sum2 && *sum1 == 0)
		return 1;
	else
		return 0;
}

//�˺�����Ϊ�������ʹ��
//�����ֶ�Patch��nop+call����������
void nop_call_1(){
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	tmp[HASH_LEN-1] = 0;
	
	error_handel();
}


//����������
int markdown_newpassword(char * hash, int * state){
	int i = 0;

#ifdef STATE_CHECK_OPEN				
	//״̬���
	check_update_state(&current_state, REQ_STATE_UPDATE_KEYFILE, STATE_UPDATE_KEYFILE);
	if(state[0] == 2 && state[1] == 1 && state[2] == 2){
		state[3] = 3;
		state[0] = 3;
	}
	else{
		return -1;
	}
#endif
	
	//���ñ�־λ
	set_flag();
	
	for(i = 0; i < KEY_FILE_NUM; i++){
		writeKeyFile(KEY_FILE_PATH[i], hash);
	}
	
	//�����־λ
	remove_flag();
	
	//�������ļ����жԱ�
	if(file_cmp(KEY_FILE_PATH, KEY_FILE_NUM) == -1){
		printf("[ERROR]\tKEYFILE BROKEN!\n");
		return -1;
	}
	
	return 0;
}


//ι���Ź�
int wcd_counter = 0;
void feed_wcd(){
	char buf[100];
	memset(buf, 0, sizeof(buf));
	//itoa(wcd_counter, buf, 10);
	sprintf(buf, "%d", wcd_counter);
	wcd_counter = (wcd_counter+1)%65536;
	
	file_cmp_write(WCD_BUF_PATH, buf);
	return;
}

int test_filecmp(){
	if(file_cmp(KEY_FILE_PATH, KEY_FILE_NUM) == -1){
		printf("[ERROR]\tKEYFILE BROKEN!\n");
		return -1;
	}
	return 0;
}





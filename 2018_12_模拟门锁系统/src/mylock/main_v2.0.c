#include<stdio.h>
#include<string.h>
#include "mylock_config.h"
#include "state_check/state_check.h"

#define DEBUG

//ģʽ����Ĭ��1Ϊ������0Ϊ�ر�
int mode_sleep = 0;
int mode_changepwd = 0;


//������
int count_sleep = 0;
int count_changepwd = 0;
int count_changepwdstate = 0;

//״̬ת��������˱���
int current_state = 0;

//��������
char input_key[KEY_LEN+2] = {0};
int count_inputkey = 0;
int check_inputkey_num = 0;

//��ǰ����ʧ�ܴ���
int current_check_psw_fail = 0;

//����salt hash����������
char hash[HASH_LEN] = {0};


//�������ݴ�����
char newhash[HASH_LEN] = {0};

void clean_all(){
	current_state = 0;
	current_check_psw_fail = 0;
	memset(input_key, 0, sizeof(input_key));
	memset(hash, 0, sizeof(hash));
	memset(newhash, 0, sizeof(newhash));	
	memset(state);
}

/* v2.0 Main
�������������ѭ���������ܼ�
*/
int main(){
	//��ʱ����
	char ch;
	int sum1 = 0, sum2 = 0, tmp, debug;
	long long time = 0;
	int state[10];
	
	printf("main begin\n");
	
	memset(state, 0, sizeof(state));

	#ifdef LOG_MODE
	freopen("mylock.log","w",stdout);  
	#endif
	
	//�����������豸���г�ʼ��
	mylock_init();
	
	//��ѭ��
	while(1){
		
		//ʱ��Ƭ���
		time = getCurrentTime();
#ifdef DEBUG		
		//printf("[INFO]\ttime: %lld\n", time);
		fflush(stdout);	
#endif		
		if(time % TIME_SLICE == 0){
				
			//ι���Ź�
			feed_wcd();

			//��˯��ģʽ����led
			if(mode_sleep == 0)
				write_buff(LED_CONTINUE);
#ifdef DEBUG		
			//printf("[INFO]\tmode_sleep: %d\n", mode_sleep);
			//fflush(stdout);	
#endif			
			//��������
			ch = get_key_or_null();
			
#ifdef DEBUG
			if(ch != 0)
				printf("[INFO]\tch: %d\n", ch);
			fflush(stdout);	
#endif				
			
			//�����룬��ʼsleep����
			if(ch == 0){
				if(count_sleep >= SLEEP_TIME_COUNT){
					mode_sleep = 1;
					write_buff(LED_SLEEP);
				}
				else
					count_sleep += 1;
				continue;
			}
			
			//���sleep���������ر�˯��ģʽ
			count_sleep = 0;
			mode_sleep = 0;
			
			//���������ʼchangepwd����
			if(ch == KEY_ENTRY_CHANGE_PWD){
#ifdef DEBUG
				printf("[DEBUG]\tCount count_changepwd! Count: %d\n", count_changepwd);
#endif				
				if(count_changepwd >= CHANGEPWD_COUNT){
#ifdef STATE_CHECK_OPEN					
					//״̬���
					check_update_state(&current_state, REQ_STATE_IN_CHANGE_PWD_MODE, STATE_IN_CHANGE_PWD_MODE);
#endif					
#ifdef DEBUG
					printf("[DEBUG]\tIn Changepwd Mode!\n");
#endif						
					count_changepwd = 0;
					mode_changepwd = 1;
					
					write_buff(LED_CHANING_PWD);
				}
				else
					count_changepwd += 1;
				continue;
			}
				
			
			//���֣������������飬�����ѹ��
			if('0'<=ch && ch<='9'){				
				input_key[count_inputkey%KEY_LEN] = ch;
				count_inputkey = (count_inputkey+1)%KEY_LEN;
				check_inputkey_num = (check_inputkey_num + ch) & 0xFF;
				input_key[(count_inputkey%KEY_LEN)] = check_inputkey_num;
				
#ifdef DEBUG
				for(debug = 0; debug < KEY_LEN + 2; debug++)
					printf("%2x ", input_key[debug]);
				printf("\n");
#endif				
				write_buff(LED_ADD_ONE_CHAR);
				
				continue;
			}
			
			//�س�����next
			if(ch == 13){
#ifdef STATE_CHECK_OPEN				
				//״̬���
				check_update_state(&current_state, REQ_STATE_ENTER_ED, STATE_ENTER_ED);
#endif				
				//��ĩβ��0���
				handel_inputkey(input_key, count_inputkey + 1);
				check_inputkey_num = 0;
				
#ifdef DEBUG
				for(debug = 0; debug < KEY_LEN + 2; debug++)
					printf("%2x ", input_key[debug]);
				printf("\n");
#endif					
				
				//��������
				if(add_salt_hash(input_key, hash, state) == -1){
#ifdef DEBUG
					printf("[INFO]\tADD_SALT_HASH ERROR!\n");
#endif	
					clean_all();
					
					write_buff(LED_PSW_WRONG);
					continue;
				}
				
#ifdef DEBUG
				printf("[INFO]\tinput_key+yama: ");
				for(debug = 0; debug < KEY_LEN + 2; debug++)
					printf("%2x", input_key[debug]);
				printf("\n");
				printf("[INFO]\tHASH: ");
				for(debug = 0; debug < HASH_LEN; debug++)
					printf("%2x", hash[debug]);
				printf("\n");
#endif	

				//����������Ϣ
				clean_array(input_key);
				count_inputkey = 0;
				
				//�ж��Ƿ�Ϊ����ģʽ
				if(mode_changepwd == 0){
										
					tmp = checkpwd(hash, &sum1, &sum2, state);
#ifdef DEBUG
					printf("[DEBUG]\tsum1: %d, sum2: %d, res: %d\n", sum1, sum2, tmp);
#endif					
					if(tmp == 1 && sum1 == sum2 && sum2 == 0){
						
#ifdef DEBUG
						printf("[DEBUG]\tOpen Succ!");
#endif						
						write_buff(LED_PSW_CORRECT);
						
						//������� ʧ�ܵĻ� ������� ��motor�ڽ���״̬���
						if(motor_up(&sum1, &sum2, state) == -1)
							error_handel();
						
						clean_all();
					}
					else{
						write_buff(LED_PSW_WRONG);
						clean_all();
					}
					
				}
				else{
					//��������ģʽ
					switch(count_changepwdstate){
						case 0: 
							// 0: ��֤����
							tmp = checkpwd(hash, &sum1, &sum2, state);
							if(tmp == 1 && sum1 == sum2 && sum2 == 0){
								count_changepwdstate = 1;
#ifdef DEBUG
								printf("[DEBUG]\tCheck Pwd Succ! You can input new password now!\n");
#endif	
								write_buff(LED_PLZ_INPUT_NEW_PSW);
							}
							else{
								mode_changepwd = 0;
#ifdef DEBUG
								printf("[DEBUG]\tCheck Pwd Fail!\n");
#endif				
								write_buff(LED_PSW_WRONG);
								clean_all();
							}
							break;
						case 1:
							// 1: ��һ������������
							
#ifdef STATE_CHECK_OPEN				
							//״̬���
							check_update_state(&current_state, REQ_STATE_SAVE_PSW_1, STATE_SAVE_PSW_1);
#endif							
							
							strcpy(newhash, hash);
							memset(hash, 0, sizeof(hash));
							count_changepwdstate = 2;
#ifdef DEBUG
							printf("[DEBUG]\tPlease Reapt, You new pwd is: ");
							for(debug = 0; debug < HASH_LEN; debug++)
								printf("%2x", newhash[debug]);
							printf("\n");
#endif				
							write_buff(LED_PLZ_REPEAT_NEW_PSW);
							memset(input_key, 0, sizeof(input_key));
							memset(hash, 0, sizeof(hash));
							break;
						case 2: 
							// 2: �ڶ�������������
							
#ifdef STATE_CHECK_OPEN				
							//״̬���
							check_update_state(&current_state, REQ_STATE_SAVE_PSW_2, STATE_SAVE_PSW_2);
#endif							
							
#ifdef DEBUG
							printf("[DEBUG]\tYou reapt pwd is: ");
							for(debug = 0; debug < HASH_LEN; debug++)
								printf("%2x", hash[debug]);
							printf("\n");
#endif							
							if(strcmp(hash, newhash) == 0){
								markdown_newpassword(hash, state);
#ifdef DEBUG
								printf("[DEBUG]\tMarkdown new pwd Succ!\n");
#endif								
								memset(newhash, 0, sizeof(newhash));
								count_changepwdstate = 0;
								mode_changepwd = 0;
								clean_all();
								write_buff(LED_PSW_CORRECT);
							}
							else{
#ifdef DEBUG								
								printf("[WRONG]\tnew password not equ! Please input again!\n");
#endif								
								count_changepwdstate = 1;
								write_buff(LED_PLZ_INPUT_NEW_PSW);
							}
							break;
						default: error_handel(); break;
					}
				}
				
				continue;
			}
			
			
			
			//�����ַ��Ļ�����ʾ��������
			write_buff(LED_PLZ_INPUT_PSW);
				
			fflush(stdout);	
		}		
	}
	
	
	return 0;
}

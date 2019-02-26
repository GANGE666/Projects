#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "led.h"
#include "../mylock_config.h"

int led_fd;
int led_last_mode = 0;
int led_last_mode_index = 0;
int led_char_num = 0;

/* v2.0
��ʼ��led
*/
int led_init(){
	//���豸 
	led_fd=open("/dev/s3c2410_led0",O_RDWR);
    if (led_fd < 0) {
        printf("[ERROR]\tLED INIT FAIL!\n * # insmod driver/s3c2410-led0.ko\n");
        return (-1);
    }
	
	//����led
	write(led_fd, TEST_BLOCK, 10);
	jmdelay(3*LED_SPEED); 
	
	printf("[INFO]\tLED TEST OVER!\n");
	
	return 0;
}


/* v2.0
led���Ƴ���
*/
int led_control(int led_mode){
	
	switch(led_mode){
		case LED_CONTINUE: 					//������ǰ״̬
			if(led_last_mode != led_mode)
				led_control(led_last_mode);
			break;

		case LED_SLEEP:						//˯��ģʽ
			if(led_last_mode == LED_SLEEP)
				break;
			write(led_fd, LED_SLEEP_BLOCK, 10);
			led_last_mode = LED_SLEEP;
			led_last_mode = 0;
			led_last_mode_index = 0;
			led_char_num = 0;
			break;
			
		case LED_PLZ_INPUT_PSW:				//��ʾ�û���������
			if(led_last_mode == LED_PLZ_INPUT_PSW){
				write(led_fd, PLEASE_ENTER_PASSWORD + (led_last_mode_index++%(LEN_PLEASE_ENTER_PASSWORD*10)), 10);
				break;
			}
			led_last_mode = LED_PLZ_INPUT_PSW;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, PLEASE_ENTER_PASSWORD + (led_last_mode_index++%(LEN_PLEASE_ENTER_PASSWORD*10)), 10);
			break;
			
		case LED_PSW_CORRECT:				//������ȷ
			led_last_mode = LED_PSW_CORRECT;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, PSW_CORRECT_BLOCK, 10);			
			break;
			
		case LED_PSW_WRONG:					//�������
			led_last_mode = LED_PSW_WRONG;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, PSW_WRONG_BLOCK, 10);			
			break;
			
		case LED_PLZ_INPUT_NEW_PSW:			//��ʾ�û�����������
			if(led_last_mode == LED_PLZ_INPUT_NEW_PSW){
				write(led_fd, NEW_PASSWORD + (led_last_mode_index++%(LEN_NEW_PASSWORD*10)), 10);
				break;
			}
			led_last_mode = LED_PLZ_INPUT_NEW_PSW;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, NEW_PASSWORD + (led_last_mode_index++%(LEN_NEW_PASSWORD*10)), 10);
			break;
			
		case LED_PLZ_REPEAT_NEW_PSW:		//��ʾ�û��ٴ�����������
			if(led_last_mode == LED_PLZ_REPEAT_NEW_PSW){
				write(led_fd, REPEAT_PASSWORD + (led_last_mode_index++%(LEN_REPEAT_PASSWORD*10)), 10);
				break;
			}
			led_last_mode = LED_PLZ_REPEAT_NEW_PSW;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, REPEAT_PASSWORD + (led_last_mode_index++%(LEN_REPEAT_PASSWORD*10)), 10);
			break;
			
		case LED_ADD_ONE_CHAR:				//������һ���ַ�
			
			led_char_num++;
			led_last_mode = LED_CONTINUE;
			write(led_fd, PSW_INPUTING_BLOCK + led_char_num * 10, 10);			
			
			break;
			
		case LED_CHANING_PWD:				//��ʾ�û������޸�����ģʽ����������
			if(led_last_mode == LED_CHANING_PWD){
				write(led_fd, CHANGING_PASSWORD_PLEASE_INPUT_OLD_PASSWORD + (led_last_mode_index++%(LEN_CHANGING_PASSWORD_PLEASE_INPUT_OLD_PASSWORD*10)), 10);
				break;
			}
			led_last_mode = LED_CHANING_PWD;
			led_last_mode_index = 0;
			led_char_num = 0;
			write(led_fd, CHANGING_PASSWORD_PLEASE_INPUT_OLD_PASSWORD + (led_last_mode_index++%(LEN_CHANGING_PASSWORD_PLEASE_INPUT_OLD_PASSWORD*10)), 10);
			break;
		
		
	}
	
	jmdelay(LED_SPEED);
	
	return 0;
}




/*
led_state_check:	ledģ����ܿ�״̬��⺯��
input:				None
output:				���յ���״̬ 
*/
int led_state_check(){
//	TODO:			�����̻߳�������״̬ 
	return 0;
}


/*
jmdelay:	��ʱ���ƺ����������״̬�仯
input: 		int n(ϣ���ӳٵ�ʱ��)
output:	 	int state(��⵽�Ƿ���Ҫ�ı�״̬)
*/
int jmdelay(int n) {
    int i,j,k,st=0;
    for (i=0;i<n;i++)
        for (j=0;j<100;j++)
            for (k=0;k<100;k++){
            	st = led_state_check();
            	if(st != 0)
            		return st;
            }
	return 0;
}


/*
display:	LED���غ���
input:		None
output:		None 
*/
int display(){
	
	int i;
	printf("\n**********LED_DISPLAY START**********\n");
    
    i = 0;
    //ѭ����ʾ/����
	while(1){
		
		//TODO: ѭ����ʾ��ʾ�����ʱ��ӱ�־or������������ʱ���� 
		write(led_fd, PLEASE_ENTER_PASSWORD + (i%(11*10)), 10);
		jmdelay(5*LED_SPEED);
		i++;
		
	} 
    
    return 0;
}





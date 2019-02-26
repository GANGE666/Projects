#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../state_check/state_check.h"

#define DCM_IOCTRL_SETPWM 			(0x10)
#define DCM_TCNTB0				(16384)

static int dcm_fd = -1;
char *DCM_DEV="/dev/s3c2410-dc-motor0";
extern int current_state;

void Delay(int t)
{
	int i;
	for(;t>0;t--)
		for(i=0;i<400;i++);
}

#define SPEED_CHECK 100*(DCM_TCNTB0/1024)

/****************************************************************/
int motor_up(int * checkvar1, int * checkvar2, int * state)
{
	int i = 0;
	int setpwm = 100;
	int factor = DCM_TCNTB0/1024;
	int motor_time = 0;
	int count = 0;
	const int speed = setpwm * factor;
	if((dcm_fd=open(DCM_DEV, O_WRONLY))<0){
		printf("Error opening %s device\n", DCM_DEV);
		return -1;
	}	
	
#ifdef STATE_CHECK_OPEN
	//状态检查
	check_update_state(&current_state, REQ_STATE_MOTOR_UP, STATE_MOTOR_UP);
	if(state[0] == 2 && state[1] == 1 && state[2] == 2){
		state[3] = 3;
		state[0] = 3;
	}
	else{
		return -1;
	}
#endif	
	
	//再次检查对比结果
	if(*checkvar1 != *checkvar2 || *checkvar1 != 0 || *checkvar2 != 0){
	  printf("unequ!\n");
	  return -1;}
	
	//检查电机速度
	if((int)SPEED_CHECK != (int)speed){
	  printf("speed uncorrect!\n");
	  printf("speed:%d\tspeed_check:%d\n",speed,SPEED_CHECK);
	  return -1;}
	
	for (; motor_time < 10; motor_time++) {
		
		if(count++ >= 10)
			break;

		ioctl(dcm_fd, DCM_IOCTRL_SETPWM, speed);			
		Delay(500);
		printf("setpwm = %d \n", setpwm);
				
	}
	
	close(dcm_fd);	
	return 0;
}

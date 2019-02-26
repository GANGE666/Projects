#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<fcntl.h>
#include"lock_guard.h"


//获取当前时间，精确到毫秒
long long getCurrentTime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

char last_wcd_buf[WCD_BUF_SIZE] = {};
int wcd_wait_time = 0;
int restart_time_sum = 0;
char keyfile_buf[MAX_FILE_SIZE];

//检查看门狗缓冲区
// 0 正常 其他 静止
int check_wcd_buf(){
	char buf[WCD_BUF_SIZE];
	int fd, readfileflag;
	
	memset(buf, 0, sizeof(buf));
	
	
	fd = open(wcd_buf_fname, O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}
	
	//读取文件，并检测是否出错 
	readfileflag = read(fd, buf, WCD_BUF_SIZE);
	if(readfileflag <= 0){
		return -1;
	}
	close(fd);	
	
	if(strcmp(last_wcd_buf, buf) == 0){
		printf("[INFO]\tDetect wcd_buf unchange!\n");
		return 1;
	}
	
	memset(last_wcd_buf, 0, sizeof(last_wcd_buf));
	stpcpy(last_wcd_buf, buf);
	
	return 0;
}

//检查keyfilecounter 
int check_keyfile_counter(){
	unsigned char buf[KEYFILE_BUFF_SIZE];
	char tmp1[FILE_PATH_SIZE], tmp2[FILE_PATH_SIZE];
	int fd, readfileflag, i;
	
	memset(buf, 0, sizeof(buf));
	
	fd = open(KEYFILE_COUNTER_PATH, O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}
	
	//读取文件，并检测是否出错 
	readfileflag = read(fd, buf, KEYFILE_BUFF_SIZE);
	if(readfileflag <= 0){
		return -1;
	}
	close(fd);	
	
	i=0;
	while(buf[i] == 0){
		if(buf[i] != MAX_CHANGE_KEYFILE[i])
			return 0;
		i++;
	}
	
	//达到更换的标准
	for(i=0; i<KEYFILE_NUM; i++){
		memset(keyfile_buf, 0, sizeof(keyfile_buf));
		file_cmp_read(KEY_FILE_PATH[i], keyfile_buf);
		//sprintf(tmp1, "touch ../mylock/keyfile/keyfile%d.new", i);
		sprintf(tmp1, "touch %s.new", KEY_FILE_PATH[i]);
		system(tmp1);
		//sprintf(tmp2, "rm ../mylock/keyfile/keyfile%d", i);
		sprintf(tmp2, "rm %s", KEY_FILE_PATH[i]);
		system(tmp2);
		//sprintf(tmp1, "mv ../mylock/keyfile/keyfile%d.new ../mylock/keyfile/keyfile%d", i, i);
		sprintf(tmp1, "mv %s.new %s", KEY_FILE_PATH[i], KEY_FILE_PATH[i]);
		system(tmp1);
		file_cmp_write(KEY_FILE_PATH[i], keyfile_buf);
	}
	
	return 0;
}

int restart_process(){
					
	printf("[WARING]\tRestarting process\n");
	system("killall mylock");
	
	file_cmp(MYLOCK_AND_BAK_PATH, MYLOCK_FILE_CHECK_NUM);
	
	check_keyfile_counter();
	
	system("mylock/mylock");
	return 0;
}

int reboot_machine(){
	printf("[ERROR]\tReboot Machine!\n");
	system("reboot");
	return 0;
}

int main(){
	int count_wcd = 0;
	printf("[INFO] Lock_Guard Start!\n");
	
	while(1){
		if(getCurrentTime() % LOCK_GUARD_TIME_SLICE == 0){
			
			
			//检查缓冲区
			if(check_wcd_buf() == 0){
				wcd_wait_time = 0;
				restart_time_sum = 0;
				continue;
			}
			
			//buf无变化
			wcd_wait_time ++;
			if(wcd_wait_time > WCD_RESTART_TIME){
				//需要重启程序
				
				restart_process();
				wcd_wait_time = 0;
				restart_time_sum ++;
				
				//重启机器
				if(restart_time_sum > MAX_TIME_RESTART_FAIL){
					
					reboot_machine();
				}
			}
						
			
		}
		
		
		
		
		
		
		
		
		
	}
	
	
	
	
	
	return 0;
}













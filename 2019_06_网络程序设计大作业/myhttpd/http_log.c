#include "httpd.h"

sem_t sem_logfile;
int LogFile_fd = 0;

void log_init(){
    sem_init(&sem_logfile, 0, 1);
    LogFile_fd = open(conf_para.LogFile, O_CREAT | O_TRUNC | O_RDWR, 0600);
    
}


void log_access(char * access){

    //printf("strlen : %d", strlen(access));
    int record_len = strlen(access);

    // 处理掉字符串中的换行
    for(int i=0; i<record_len; i++)
        if(access[i] == '\n')
            access[i] = ' ';
    
    access[record_len - 1] = '\n';

    //申请锁
    sem_wait(&sem_logfile);

    int size = write(LogFile_fd, access, record_len);
    if(size <= 0){
        perror("Cannot write to log file!\n");
    }
    //dbg_printf(access);
    //释放锁
    sem_post(&sem_logfile);

    return;
}


void log_release(){
    sem_destroy(&sem_logfile);
    close(LogFile_fd);
}




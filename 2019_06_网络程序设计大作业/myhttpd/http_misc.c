#include <time.h>
#include<stdio.h>   

void get_time(char * record) {
    time_t now; //实例化time_t结构    
    struct tm *timenow; //实例化tm结构指针    
    time(&now);   
    
    timenow = localtime(&now);   
    
    sprintf(record, "%s",asctime(timenow));   
}


int file_size(char* filename)  
{  
    FILE *fp=fopen(filename,"r");  
    if(!fp) return -1;  
    fseek(fp,0L,SEEK_END);  
    int size=ftell(fp);  
    fclose(fp);  
      
    return size;  
}

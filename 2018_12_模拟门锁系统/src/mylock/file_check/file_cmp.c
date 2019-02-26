
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include"../mylock_config.h"

#define MAX_CMP_FILE 10
#define MAX_FILE_SIZE 1000

#define XOR_KEY 0xAA

#ifndef DEBUG
	#define DEBUG
#endif

// 输入为文件名数组，及文件数量 
int file_cmp(char * f_name[], int f_num){
	
	int i, j, k, debug, checkcode;
	int begin_fnum = 0, cmped_sum = 1;
	char tmp1[MAX_FILE_SIZE], tmp2[MAX_FILE_SIZE];
	
	//已对比过的文件编号
	int cmped_num_array[MAX_CMP_FILE];
	int cmped_num = 0; 
	
	// 相同则置位为前一文件的编号 
	int cmp_res[MAX_CMP_FILE];
	memset(cmp_res, -1, sizeof(cmp_res));
	
	while(begin_fnum < f_num){
		
		cmp_res[begin_fnum] = begin_fnum;
		printf("%s\n",f_name[begin_fnum]);
		memset(tmp1, 0, sizeof(tmp1));
		if(file_cmp_read(f_name[begin_fnum], tmp1) != 1){
				// 读文件失败
				printf("file_cmp.c\tRead File Fail!\n");
				return -1;
		}
#ifdef DEBUG
		printf("f_name: %s \tcontent: %s\n", f_name[begin_fnum], tmp1);
#endif		
		//循环检查 
		for(i=begin_fnum+1; i<f_num; i++){
			
			memset(tmp2, 0, sizeof(tmp2));
			
			if(file_cmp_read(f_name[i], tmp2) == 1)
			{
#ifdef DEBUG
				printf("f_name: %s \tcontent: %s\n", f_name[i], tmp2);
#endif	
				if(strcmp(tmp1, tmp2) == 0)
				{
					//相同
					cmp_res[i] = begin_fnum; 
					cmped_sum ++;
				}
			}
			else
			{
				// 读文件失败
				printf("file_cmp.c\tRead File Fail!\n");
				return -1; 
			}	
			
		}
		
		//将检查过的文件保存起来 
		cmped_num_array[cmped_num++] = begin_fnum;
		
#ifdef DEBUG
		for(debug = 0; debug < f_num; debug++){
			printf("f_name: %s \tcmp_res: %d\n", f_name[debug], cmp_res[debug]);
		}
		printf("cmped_sum: %d \tf_num: %d\n", cmped_sum, f_num);
#endif	

		//推荐f_num为奇数，如f_num=3，f_num/2=1，当cmped_sum 大于 1时才为正确的文件
		if(cmped_sum > f_num/2){
			//此文件是正确的
			
#ifdef DEBUG
			printf("file_cmp:\t%s is right! cmped_sum = %d; f_num = %d\n", f_name[begin_fnum], cmped_sum, f_num);
#endif			

			#ifdef KEYFILE_CHECK_CODE

				checkcode = 0;
				i = 0;

				while(tmp1[i+1] != 0){
					checkcode += tmp1[i] ^ XOR_KEY;
					i ++;
				}

				if(tmp1[i] ^ XOR_KEY != checkcode & 0xFF)
				{
					printf("File Check Code Uncompare\n");
					return -1;
				}
			
			#endif


			if(cmped_sum != f_num){
				//有文件出错 
				for(i = 0; i < f_num; i++)
					if(cmp_res[i] != begin_fnum){
						//对错误文件进行重写
						file_cmp_write(f_name[i], tmp1); 
#ifdef DEBUG
						printf("Repair file: %s by %s\n", f_name[i], f_name[begin_fnum]);
#endif							
					}				
			} 
			//修改完成后，返回正常 
			return 0;
		}
		else{
			//此文件不对，找到下一个不同的文件继续循环检查
			
#ifdef DEBUG
			printf("file_cmp:\t%s is WRONG! cmped_sum = %d; f_num = %d\n", f_name[begin_fnum], cmped_sum, f_num);
#endif
			 
			for(i = begin_fnum + 1; i < f_num; i++){
				if(cmp_res[i] != begin_fnum){
					k = 0;
					//检查是否已经检查过了 
					for(j = 0; j < cmped_num; j++)
						if(cmp_res[i] == cmped_num_array[j]){
							k = 1;
							break;
						}
					//为检查过，开始下一次检查 
					if(k == 0){
						begin_fnum = i;
						cmped_sum = 1;
						break;
					}
				}
			}			 
		}
	
	}
		
	
	//超过半数的文件都不对 
	return -1;
}

// 返回1表示读取成功，-1失败 
int file_cmp_read(char * f_name, char * buf){
	//打开文件，常规操作 
	int fd, readfileflag;
	printf("opening %s\n",f_name);
	fd = open(f_name, O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}
	printf("open succ!\n");
	//读取文件，并检测是否出错 
	readfileflag = read(fd, buf, MAX_FILE_SIZE);
	if(readfileflag <= 0){
	  close(fd);
		return -1;
	}
	close(fd);
	return 1;
}

int file_cmp_write(char * f_name, char * buf){
	//打开文件，常规操作 
	int fd, readfileflag;
	fd = open(f_name, O_WRONLY);
	if(fd < 0)
	{
		return -1;
	}
	
	//写文件，并检测是否出错 
	readfileflag = write(fd, buf, MAX_FILE_SIZE);
	if(readfileflag <= 0){
		return -1;
	}
	close(fd);
	return 1;
}

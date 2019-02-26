
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            klib.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


/*======================================================================*
                               itoa
 *======================================================================*/
PUBLIC char * itoa(char * str, int num)/* 数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800 */
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = FALSE;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = TRUE;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}


/*======================================================================*
                               disp_int
 *======================================================================*/
PUBLIC void disp_int(int input)
{
	char output[16];
	itoa(output, input);
	disp_str(output);
}

/*======================================================================*
                               delay
 *======================================================================*/
PUBLIC void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++){
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<10;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}


/*======================================================================*
                             在指定位置显示字符串
 *======================================================================*/
PUBLIC void disp_str_with_pos(int line, int row, char * str){
	while(semaphore_disp_pos != 1){}
	semaphore_disp_pos = 0;

	saved_disp_pos = disp_pos;
	disp_pos = 160*line + row*2;

	disp_str(str);

	disp_pos = saved_disp_pos;
	semaphore_disp_pos = 1;
}


PUBLIC void disp_int_with_pos(int line, int row, int n){
	while(semaphore_disp_pos != 1){}
	semaphore_disp_pos = 0;

	saved_disp_pos = disp_pos;
	disp_pos = 160*line + row*2;

	disp_int(n);

	disp_pos = saved_disp_pos;
	semaphore_disp_pos = 1;
}

PUBLIC void disp_str_with_po_without_semaphore(int line, int row, char * str){
	int tmp = disp_pos;
	disp_pos = 160*line + row*2;

	disp_str(str);

	disp_pos = tmp;
}

PUBLIC void disp_int_with_po_without_semaphore(int line, int row, int n){
	int tmp = disp_pos;
	disp_pos = 160*line + row*2;

	disp_int(n);

	disp_pos = tmp;
}

PUBLIC void debug_print_tasklink(){
	int i=0, tmp = disp_pos;
	disp_pos = 160 * 23;
	for(i=0;i<10;i++){
		disp_int(ready_proc_list[i][0]);
		disp_pos+=8;
	}
	disp_pos = 160*24;
	for(i=0;i<10;i++){
		disp_int(ready_proc_list[i][1]);
		disp_pos+=8;
	}
	disp_pos = tmp;
}
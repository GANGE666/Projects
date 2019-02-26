#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

//int in_keyboard_handler = 1;

PUBLIC void keyboard_handler(int irq)
{
	int n, i, j, ready;
	PROCESS* p;
	u8 ch = in_byte(0x60);
	//if(in_keyboard_handler == 0)return;
/*
	if(ch == 0xB){
		current_task_num = (current_task_num) % 3 + 1;
	}
*/
	//disp_int_with_pos(0,1,ch);

	//disp_int_with_po_without_semaphore(20, 20, ch);

	if(0x2 <= ch && ch <= 0xA){ // 输入为数字 1-9
		n = ch - 1; // 将扫描码2-10转化为数字1-9

		// 判断是应该就绪或挂起进程
		ready = 0;
		j = 0;
		for (i = 1; i < current_task_num; i++)
		{
			j = ready_proc_list[j][0];
			if(j == n){
				ready = 1;
			}
		}
	
		//disp_int_with_po_without_semaphore(20, 30, semaphore_proc_num);

		if(semaphore_proc_num != 1)//return;	//防止在进程调度时发生冲突
		semaphore_proc_num = 0;

		//disp_int_with_po_without_semaphore(20, 0, ready_proc_list[0][0]);
		//disp_int_with_po_without_semaphore(20, 10, ready_proc_list[0][1]);		

		if(ready == 0){
			// 使进程就绪

			//6号以上没有while循环，需要重新填充pcb
			if(n >= 6){
				disp_int_with_pos(n*2+2, 8, get_ticks());
				re_full_pcb(n);
			}

			//添加进程至链表尾
			ready_proc_list[n][0] = ready_proc_list[j][0];
			ready_proc_list[n][1] = j;

			ready_proc_list[j][0] = n;	//j的下一个是n
			ready_proc_list[ready_proc_list[n][0]][1] = n;	//n的下一个的上一个是n

			current_task_num ++;

			//debug_print_tasklink();
			disp_str_with_pos(21, n*8, "Ready ");
		}
		else{
			//挂起进程
			current_task_num --;

			ready_proc_list[ready_proc_list[n][1]][0] = ready_proc_list[n][0];
			ready_proc_list[ready_proc_list[n][0]][1] = ready_proc_list[n][1];

			p = proc_table;
			p_proc_ready = (p+ready_proc_list[n][0]);

			disp_str_with_pos(21, n*8, "HangOn");
		}


		semaphore_proc_num = 1;
	}


	if(ch == 0xB)		//按键0
		clean_disp_keyboard();

	return;
}


void clean_disp_keyboard(){
	int i;
	disp_pos = 0;
	for (i = 0; i < 80*25; i++) {
		disp_str(" ");
	}
	disp_pos = 160*10;
}


#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/*
动态优先级计算公式：
当前优先级 = CPU使用时间/2 + 基本优先级
所以数字越大 优先程度越低
最大 INF，防溢出
*/

#define CALCA_BASE_PRIO		0x500
#define CALCB_BASE_PRIO 	0x700
#define CALCC_BASE_PRIO 	0x900
#define CALCD_BASE_PRIO		0x1200

#define DYNAIC_PRIO_TIME_SLICE	50

int proc_base_prio[NR_TASKS] = {
	INF-1, INF-1, INF-1, INF-1, INF-1, INF-1, CALCA_BASE_PRIO, CALCB_BASE_PRIO, CALCC_BASE_PRIO, CALCD_BASE_PRIO
};



//要让优先度高的程序运行得越久 其优先级越低
void dynaic_prio_init(){
	int i;

	for(i = 0; i < NR_TASKS; i++){
		proc_table[i].ticks = DYNAIC_PRIO_TIME_SLICE;
		proc_table[i].priority =  proc_base_prio[i];
	}

	hope_use_time[6] = CALCD_RUNTIME;
	hope_use_time[7] = CALCC_RUNTIME;
	hope_use_time[8] = CALCB_RUNTIME;
	hope_use_time[9] = CALCA_RUNTIME;

	return;
}


PUBLIC void schedule_DYNAMIC_PRIO(){
	PROCESS* p;
	int  i, tmp_p = 0;
	int	 smallest_prio = INF, smallest_proc = 0;
	int  greatest_ticks = 0;

	p = proc_table;
	disp_int_with_pos(0, 8, get_ticks());
	disp_int_with_po_without_semaphore(22, 0, current_task_num);
	disp_int_with_po_without_semaphore(22, 10, p_proc_ready->pid);
	debug_print_tasklink();

	disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "   ");
	
	//更新当前进程优先级
	if(p_proc_ready->priority < INF-0x1000)
		p_proc_ready->priority = used_cpu_time[p_proc_ready->pid]/2 + proc_base_prio[p_proc_ready->pid];


	while(!greatest_ticks){

		tmp_p = 0;

		for(i = 0; i < current_task_num; i++){

			// 找到下一个进程
			tmp_p = ready_proc_list[tmp_p][0];

				if((p+tmp_p)->ticks > 0 && smallest_prio > (proc_table+tmp_p)->priority)
				{
					greatest_ticks = 1;
					smallest_prio = (proc_table+tmp_p)->priority;
					smallest_proc = tmp_p;
				}

		}
		
		//当每个进程时间片都为0时
		if(!greatest_ticks){
			for(i = 0; i < NR_TASKS; i++){
				(p+i)->ticks = DYNAIC_PRIO_TIME_SLICE;
			}
			disp_int_with_po_without_semaphore(0, 30, get_ticks());
		}

	}

	if(current_task_num > 1){
		disp_str_with_pos(smallest_proc*2+2, 72, "RUN");
		p_proc_ready = (p+smallest_proc);
	}
	disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "RUN");

	return;
	
}
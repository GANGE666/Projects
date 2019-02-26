
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
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
                              schedule
 *======================================================================*/
/*
PUBLIC void schedule()
{
	PROCESS* p;
	int	 greatest_ticks = 0;

	while (!greatest_ticks) {
		for (p = proc_table; p < proc_table+current_task_num; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}

		if (!greatest_ticks) {
			for (p = proc_table; p < proc_table+current_task_num; p++) {
				p->ticks = p->priority;
			}
		}
	}
}
*/
int counter = 0;
// 使用链表的调度算法
PUBLIC void schedule()
{
	PROCESS* p;
	int i, tmp_p = 0;
	int	 greatest_ticks = 0;

	disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "   ");
	
	while (!greatest_ticks) {
		tmp_p = 0;

		//if(semaphore_proc_num != 1) return;	//防止在进程调度时发生冲突
		//semaphore_proc_num = 0;

		p = proc_table;
		disp_int_with_pos(0, 8, get_ticks());
		disp_int_with_po_without_semaphore(22, 0, current_task_num);
		debug_print_tasklink();
		//disp_str_with_po_without_semaphore(22, 30, p_proc_ready->p_name);
		//disp_int_with_po_without_semaphore(18, 60, (p)->ticks);
		//disp_int_with_po_without_semaphore(19, 60, (p+2)->ticks);
		for(i = 0; i < current_task_num; i++){

			
			//disp_int_with_po_without_semaphore(22, 10, tmp_p);
			//disp_int_with_po_without_semaphore(22, 20, (p+tmp_p)->ticks);

			// 找到下一个进程
			tmp_p = ready_proc_list[tmp_p][0];

			if((p+tmp_p)->ticks > greatest_ticks){
				greatest_ticks = (p+tmp_p)->ticks;
				p_proc_ready = (p+tmp_p);
				//disp_int_with_po_without_semaphore(22, 70, tmp_p);
			}
			//else 
				//disp_int_with_po_without_semaphore(22, 60, counter++);
		}

		//semaphore_proc_num = 1;

		if (!greatest_ticks) {
			for(i = 0; i < NR_TASKS; i++){
				(p+i)->ticks = (p+i)->priority;
			}
			//disp_int_with_po_without_semaphore(21, 60, counter++);
		}
		//disp_int_with_po_without_semaphore(22, 60, counter++);
	}

	disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "RUN");
}


PUBLIC void schedule_SFJ(){
	PROCESS* p;
	int i, tmp_p = 0;
	int	 smallest_time = INF, smallest_proc = 0;

	p = proc_table;
	disp_int_with_pos(0, 8, get_ticks());
	disp_int_with_po_without_semaphore(22, 0, current_task_num);
	disp_int_with_po_without_semaphore(22, 10, p_proc_ready->pid);
	debug_print_tasklink();

	for(i = 1; i < current_task_num; i++){

		// 找到下一个进程
		tmp_p = ready_proc_list[tmp_p][0];

		disp_int_with_po_without_semaphore(22, 20, smallest_time);
		disp_int_with_po_without_semaphore(tmp_p*2+2, 40, hope_use_time[tmp_p] - used_cpu_time[tmp_p]);

		if(smallest_time > hope_use_time[tmp_p] - used_cpu_time[tmp_p]){ //运行时间-已运行时间得到剩余时间
			smallest_time = hope_use_time[tmp_p] - used_cpu_time[tmp_p];
			smallest_proc = tmp_p;			
		}
	}

	if(current_task_num > 1 && smallest_proc != p_proc_ready->pid && smallest_proc != 0){
		disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "   ");
		disp_str_with_pos(tmp_p*2+2, 72, "Run");
		p_proc_ready = (p+tmp_p);
	}

	disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "Run");
	return;
}






/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}


PUBLIC int sys_exit_current_proc()
{
	int n;
	PROCESS* p;

	n = p_proc_ready->pid;

	//挂起进程
	current_task_num --;

	ready_proc_list[ready_proc_list[n][1]][0] = ready_proc_list[n][0];
	ready_proc_list[ready_proc_list[n][0]][1] = ready_proc_list[n][1];

	p = proc_table;
	p_proc_ready = (p+ready_proc_list[n][0]);

	disp_str_with_pos(n*2+2, 72, "   ");
	disp_str_with_pos(21, n*8, "Finish");
	return 0;
}


PUBLIC void re_full_pcb(int i)
{
	//disp_str_with_pos(20, i*8, "Re_Full_");
	TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;

	p_task_stack -= (p_task->stacksize)*(i);
	p_proc += i;
	p_task += i;
	selector_ldt += (1 << 3)*(i);


	strcpy(p_proc->p_name, p_task->name);	// name of the process
	p_proc->pid = i;			// pid

	p_proc->ldt_sel = selector_ldt;

	memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
	       sizeof(DESCRIPTOR));
	p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
	memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
	       sizeof(DESCRIPTOR));
	p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
	p_proc->regs.cs	= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.ds	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.es	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.fs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.ss	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
		| RPL_TASK;

	p_proc->regs.eip = (u32)p_task->initial_eip;
	p_proc->regs.esp = (u32)p_task_stack;
	p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

	used_cpu_time[i] = 0;

	return;
}







/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
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

#define SLEEP_TIME 500
#define WAIT_TIME 50

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++) {
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

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	proc_table[0].ticks = proc_table[0].priority =  1;
	proc_table[1].ticks = proc_table[1].priority = 25;
	proc_table[2].ticks = proc_table[2].priority =  3;
	proc_table[3].ticks = proc_table[3].priority =  3;
	proc_table[4].ticks = proc_table[4].priority =  3;
	proc_table[5].ticks = proc_table[5].priority =  3;
	proc_table[6].ticks = proc_table[6].priority = 25;
	proc_table[7].ticks = proc_table[7].priority = 25;
	proc_table[8].ticks = proc_table[8].priority = 25;
	proc_table[9].ticks = proc_table[9].priority = 25;

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

        /* 初始化 8253 PIT */
        out_byte(TIMER_MODE, RATE_GENERATOR);
        out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
        out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));

        put_irq_handler(CLOCK_IRQ, clock_handler); /* 设定时钟中断处理程序 */
        enable_irq(CLOCK_IRQ);                     /* 让8259A可以接收时钟中断 */

        put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
        enable_irq(KEYBOARD_IRQ);

    disp_pos = 0;
	for (i = 0; i < 80*25; i++) {
		disp_str(" ");
	}
	disp_pos = 160*10;

	for(i = 0; i < NR_TASKS; i++){
		ready_proc_list[i][0] = 0;
		ready_proc_list[i][1] = 0;
	}

	for(i = 0; i < NR_TASKS; i++){
		used_cpu_time[i] = 0;
		hope_use_time[i] = INF;
	}
	hope_use_time[6] = CALCA_RUNTIME;
	hope_use_time[7] = CALCB_RUNTIME;
	hope_use_time[8] = CALCC_RUNTIME;
	hope_use_time[9] = CALCD_RUNTIME;

	if(current_schedule_mode == DYNAMIC_PRIO)
		dynaic_prio_init();

	disp_current_mode();

	restart();

	while(1){}
}

/*======================================================================*
                        System Idle Process
 *======================================================================*/
void System_Idle_Process()
{
	while (1) {
		//disp_str(".");
		//milli_delay(SLEEP_TIME*25);
	}
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int i = 0;
	while (1) {
		disp_str("A.");
		milli_delay(SLEEP_TIME);
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	int i = 0x1000;
	int pos = 0;
	while(1){

		disp_str_with_pos(4, pos, "B.");
		pos += 2;

		milli_delay(SLEEP_TIME);
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestC()
{
	int i = 0x2000;
	int pos = 0;
	while(1){
		
		disp_str_with_pos(6,pos,"C.");
		pos += 2;
		milli_delay(SLEEP_TIME);
	}
}


/*======================================================================*
                               Producer
 *======================================================================*/
void Producer()
{
	int line = 4;
	int row = 0;

	disp_str_with_pos(line, row, "Producer");
	row += 8;

	while(1){

		//缓冲区满
		while(goods_buff == 0xFF) milli_delay(WAIT_TIME);

		//disp_str_with_pos(18, 0, "Producer sema: ");
		//disp_int_with_pos(18, 20, semaphore_goods_buff);
		while(semaphore_goods_buff != 1){};
		semaphore_goods_buff = 0;

		goods_buff++;
		disp_int_with_pos(0, 80, goods_buff);
		disp_str_with_pos(line, row++, "+");

		semaphore_goods_buff = 1;

		milli_delay(SLEEP_TIME);
		clear_disp(line, &row);
	}
}

/*======================================================================*
                               Consumer
 *======================================================================*/
void Consumer_1()
{
	int line = 6;
	int row = 0;

	disp_str_with_pos(line, row, "Consumer1");
	row += 8;

	while(1){
		//disp_str_with_pos(19, 0, "Consumer sema: ");
		//disp_int_with_pos(19, 20, semaphore_goods_buff);
		while(semaphore_consumer != 1){};
		semaphore_consumer = 0;

		while(goods_buff == 0) milli_delay(WAIT_TIME);

		while(semaphore_goods_buff != 1){};
		semaphore_goods_buff = 0;

		goods_buff--;
		disp_int_with_pos(0, 80, goods_buff);
		disp_str_with_pos(line, row++, "-");

		semaphore_goods_buff = 1;

		semaphore_consumer = 1;

		milli_delay(SLEEP_TIME);
		clear_disp(line, &row);
	}
}
void Consumer_2()
{
	int line = 8;
	int row = 0;

	disp_str_with_pos(line, row, "Consumer2");
	row += 8;

	while(1){
		while(semaphore_consumer != 1){};
		semaphore_consumer = 0;

		while(goods_buff == 0) milli_delay(WAIT_TIME);

		while(semaphore_goods_buff != 1){};
		semaphore_goods_buff = 0;

		goods_buff--;
		disp_int_with_pos(0, 80, goods_buff);
		disp_str_with_pos(line, row++, "-");

		semaphore_goods_buff = 1;

		semaphore_consumer = 1;

		milli_delay(SLEEP_TIME);
		clear_disp(line, &row);
	}
}
void Consumer_3()
{
	int line = 10;
	int row = 0;

	disp_str_with_pos(line, row, "Consumer3");
	row += 8;

	while(1){
		while(semaphore_consumer != 1){};
		semaphore_consumer = 0;

		while(goods_buff == 0) milli_delay(WAIT_TIME);

		while(semaphore_goods_buff != 1){};
		semaphore_goods_buff = 0;

		goods_buff--;
		disp_int_with_pos(0, 80, goods_buff);
		disp_str_with_pos(line, row++, "-");

		semaphore_goods_buff = 1;

		semaphore_consumer = 1;

		milli_delay(SLEEP_TIME);
		clear_disp(line, &row);
	}
}
void Consumer_4()
{
	int line = 12;
	int row = 0;

	disp_str_with_pos(line, row, "Consumer4");
	row += 8;

	while(1){
		while(semaphore_consumer != 1){};
		semaphore_consumer = 0;

		while(goods_buff == 0) milli_delay(WAIT_TIME);

		while(semaphore_goods_buff != 1){};
		semaphore_goods_buff = 0;

		goods_buff--;
		disp_int_with_pos(0, 80, goods_buff);
		disp_str_with_pos(line, row++, "-");

		semaphore_goods_buff = 1;

		semaphore_consumer = 1;

		milli_delay(SLEEP_TIME);
		clear_disp(line, &row);
	}
}
/*======================================================================*
                               calc test
 *======================================================================*/

// 显示为
// 进程名	提交时间		运行时间 	开始时间 	完成时间

// pid = 6
void Calca()
{
	int line = 14;
	int row = 0;
	int pid = 6;
	//显示开始运行的时间
	disp_int_with_pos(line, row+16, hope_use_time[pid]);
	disp_int_with_pos(line, row+24, get_ticks());

	disp_str_with_pos(line, row, "P1");

	if(current_schedule_mode == DYNAMIC_PRIO)
		disp_str_with_pos(line+1, row, "Current prio: ");

	//milli_delay(SLEEP_TIME*10);

	while(used_cpu_time[6] < hope_use_time[6]){
		milli_delay(WAIT_TIME);
		if(current_schedule_mode == DYNAMIC_PRIO)
			disp_int_with_pos(line+1, row+16, p_proc_ready->priority);
	};

	disp_int_with_pos(line, row+56, get_ticks());
	exit_proc();

}


void Calcb()
{
	int line = 16;
	int row = 0;
	int pid = 7;
	//显示开始运行的时间
	disp_int_with_pos(line, row+16, hope_use_time[pid]);
	disp_int_with_pos(line, row+24, get_ticks());

	disp_str_with_pos(line, row, "P2");

	if(current_schedule_mode == DYNAMIC_PRIO)
		disp_str_with_pos(line+1, row, "Current prio: ");

	//milli_delay(SLEEP_TIME*10);
	while(used_cpu_time[7] < hope_use_time[7]){
		milli_delay(WAIT_TIME);
		if(current_schedule_mode == DYNAMIC_PRIO)
			disp_int_with_pos(line+1, row+16, p_proc_ready->priority);
	};

	disp_int_with_pos(line, row+56, get_ticks());
	exit_proc();

}

void Calcc()
{
	int line = 18;
	int row = 0;
	int pid = 8;
	//显示开始运行的时间
	disp_int_with_pos(line, row+16, hope_use_time[pid]);
	disp_int_with_pos(line, row+24, get_ticks());

	disp_str_with_pos(line, row, "P3");

	if(current_schedule_mode == DYNAMIC_PRIO)
		disp_str_with_pos(line+1, row, "Current prio: ");

	//milli_delay(SLEEP_TIME*10);
	while(used_cpu_time[8] < hope_use_time[8]){
		milli_delay(WAIT_TIME);
		if(current_schedule_mode == DYNAMIC_PRIO)
			disp_int_with_pos(line+1, row+16, p_proc_ready->priority);
	};

	disp_int_with_pos(line, row+56, get_ticks());
	exit_proc();

}

void Calcd()
{
	int line = 20;
	int row = 0;
	int pid = 9;
	//显示开始运行的时间
	disp_int_with_pos(line, row+16, hope_use_time[pid]);
	disp_int_with_pos(line, row+24, get_ticks());

	disp_str_with_pos(line, row, "P4");

	if(current_schedule_mode == DYNAMIC_PRIO)
		disp_str_with_pos(line+1, row, "Current prio: ");

	//milli_delay(SLEEP_TIME*10);
	while(used_cpu_time[9] < hope_use_time[9]){
		milli_delay(WAIT_TIME);
		if(current_schedule_mode == DYNAMIC_PRIO)
			disp_int_with_pos(line+1, row+16, p_proc_ready->priority);
	};

	disp_int_with_pos(line, row+56, get_ticks());
	exit_proc();

}





void clear_disp(int line, int * row){
	int i = 0;
	if(*row <= 80-8)
		return;
	*row = 8;
	for(i = 0; i < 80-8; i++)
		disp_str_with_pos(line, (*row)+i, " ");
	return;
}


void disp_current_mode(){
	switch(current_schedule_mode){
		case NORMAL:
			disp_str_with_pos(0, 50, "In normal mode");
			break;
		case FCFS:
			disp_str_with_pos(0, 50, "In FCFS mode");
			break;
		case SFJ:
			disp_str_with_pos(0, 50, "In SFJ mode");
			break;
		case DYNAMIC_PRIO:
			disp_str_with_pos(0, 50, "In dynamic priority mode");
			break;
		default:
			disp_str_with_pos(0, 50, "ERROR! Unkown Mode!!");
	}
	return;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"


PUBLIC	PROCESS			proc_table[NR_TASKS];

PUBLIC	char			task_stack[STACK_SIZE_TOTAL];

PUBLIC	TASK	task_table[NR_TASKS] = {{System_Idle_Process, STACK_SIZE_TESTA, "System_Idle_Process"},
					{Producer, STACK_SIZE_TESTB, "Producer"},
					{Consumer_1, STACK_SIZE_TESTC, "Consumer_1"},
					{Consumer_2, STACK_SIZE_TESTC, "Consumer_2"},
					{Consumer_3, STACK_SIZE_TESTC, "Consumer_3"},
					{Consumer_4, STACK_SIZE_TESTC, "Consumer_4"},
					{Calca, 	 STACK_SIZE_TESTC, "Calca"},
					{Calcb, 	 STACK_SIZE_TESTC, "Calcb"},
					{Calcc, 	 STACK_SIZE_TESTC, "Calcc"},
					{Calcd, 	 STACK_SIZE_TESTC, "Calcd"}
				};

PUBLIC	irq_handler		irq_table[NR_IRQ];

PUBLIC	system_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks, sys_exit_current_proc};

// 当前进程数，调度算法使用
PUBLIC	int 		current_task_num = 1;
PUBLIC 	int 		current_schedule_mode = NORMAL;

PUBLIC	int 		semaphore_disp_pos = 1;
PUBLIC 	int 		saved_disp_pos = 0;

// 生产者消费者缓冲区
PUBLIC 	int			goods_buff = 0x80;
PUBLIC	int 		semaphore_goods_buff = 1;

PUBLIC 	int 		semaphore_consumer = 1;

// 记录就绪状态的进程，以及进程链表，及信号量
PUBLIC	int 		semaphore_proc_num = 1;
PUBLIC 	int 		ready_proc_num = 1;
PUBLIC 	int 		ready_proc_list[NR_TASKS][2] = {
	{0},	// next proc
	{0}		// front proc
};

//占用cpu时间
PUBLIC 	int 		used_cpu_time[NR_TASKS] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//运行时间
PUBLIC 	int 		hope_use_time[NR_TASKS] = {
	INF, INF, INF, INF, INF, INF, CALCA_RUNTIME, CALCB_RUNTIME, CALCC_RUNTIME, CALCD_RUNTIME
};





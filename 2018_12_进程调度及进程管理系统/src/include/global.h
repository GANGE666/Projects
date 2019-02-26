
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* EXTERN is defined as extern except in global.c */
#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

EXTERN	int		ticks;

EXTERN	int		disp_pos;
EXTERN	u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN	GATE		idt[IDT_SIZE];

EXTERN	u32		k_reenter;

EXTERN	TSS		tss;
EXTERN	PROCESS*	p_proc_ready;

extern 	int 		current_schedule_mode;

extern	int 		current_task_num;
extern 	int 		semaphore_disp_pos;
extern 	int 		saved_disp_pos;

extern 	int 		goods_buff;
extern 	int			semaphore_goods_buff;
extern 	int			semaphore_consumer; 

extern	PROCESS		proc_table[];
extern	char		task_stack[];
extern  TASK            task_table[];
extern	irq_handler	irq_table[];

extern 	int 		semaphore_proc_num;
extern 	int 		ready_proc_num;
extern 	int			ready_proc_list[NR_TASKS][2];

extern 	int 		used_cpu_time[NR_TASKS];
extern 	int 		hope_use_time[NR_TASKS];


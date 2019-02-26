
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* klib.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);
PUBLIC void disp_str_with_pos(int row, int line, char * str);
PUBLIC void disp_int_with_pos(int row, int line, int n);
PUBLIC void disp_str_with_po_without_semaphore(int line, int row, char * str);
PUBLIC void disp_int_with_po_without_semaphore(int line, int row, int n);
PUBLIC void debug_print_tasklink();

PUBLIC void schedule_SFJ();

/* protect.c */
PUBLIC void	init_prot();
PUBLIC u32	seg2phys(u16 seg);

/* klib.c */
PUBLIC void	delay(int time);

/* kernel.asm */
void restart();

/* main.c */
void System_Idle_Process();
void TestA();
void TestB();
void TestC();

void Producer();
void Consumer_1();
void Consumer_2();
void Consumer_3();
void Consumer_4();
void Calca();
void Calcb();
void Calcc();
void Calcd();

/* i8259.c */
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq(int irq);

/* clock.c */
PUBLIC void clock_handler(int irq);

PUBLIC void keyboard_handler(int irq);

/* 以下是系统调用相关 */

/* proc.c */
PUBLIC  int     sys_get_ticks();        /* sys_call */
PUBLIC 	int 	sys_exit_current_proc();

/* syscall.asm */
PUBLIC  void    sys_call();             /* int_handler */
PUBLIC  int     get_ticks();
PUBLIC 	int 	exit_proc();


PUBLIC void re_full_pcb(int pid);


PUBLIC void schedule_DYNAMIC_PRIO();
PUBLIC void dynaic_prio_init();


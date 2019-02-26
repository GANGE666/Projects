
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
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
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{
	ticks++;
	p_proc_ready->ticks--;

	//disp_int_with_pos(0, 8, ticks);

	//记录当前进程占用cpu的时间
	used_cpu_time[p_proc_ready->pid]++;

	if (k_reenter != 0) {
		return;
	}

	if (p_proc_ready->ticks > 0) {
		return;
	}


	switch(current_schedule_mode){
		case NORMAL:
			schedule();
			break;

		case FCFS:
			if(p_proc_ready->pid == 0) 				//避免0号进程占用
				p_proc_ready = proc_table + ready_proc_list[0][0];
			disp_str_with_pos((p_proc_ready->pid)*2+2, 72, "Run");
			return;

		case SFJ:
			schedule_SFJ();
			break;

		case DYNAMIC_PRIO:
			schedule_DYNAMIC_PRIO();
			break;

		default: schedule();
	}




}

/*======================================================================*
                              milli_delay
 *======================================================================*/
PUBLIC void milli_delay(int milli_sec)
{
        int t = get_ticks();

        while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}


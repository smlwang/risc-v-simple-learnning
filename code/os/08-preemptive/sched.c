#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

struct {
    // task alive
    uint8_t flag;

    uint8_t priority;
    uint32_t timeslice;
    uint32_t timelast;
} task_stat[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _tot = 0;
static int _current = -1;

void sched_init()
{
	w_mscratch(0);

	/* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
}

int sched_task() {
    int sel = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_stat[i].flag == 0) {
            continue;
        }
        if (sel == -1) {
            sel = i;
        }
        if (task_stat[i].priority > task_stat[sel].priority) {
            sel = i;
        }
    }
    return sel;
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule(void)
{
	if (_tot <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}
	_current = sched_task();
	struct context *next = &(ctx_tasks[_current]);
	switch_to(next);
}

void tirs_shed() {
	if (_tot <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}
    if (--task_stat[_current].timelast <= 0) {
        task_stat[_current].flag = 0;
    }
    printf("current last time : %d\n", task_stat[_current].timelast);
    schedule();
}

int find_free_task() {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!task_stat[i].flag) {
            return i;
        }
    }
    return -1;
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routune entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*task)(void* param), 
                void *param,
                uint8_t priority,
                uint32_t timeslice)
{
    int sel = find_free_task();
	if (sel >= 0) {
		ctx_tasks[sel].sp = (reg_t) &task_stack[sel][STACK_SIZE - 1];
		ctx_tasks[sel].pc = (reg_t) task;
        ctx_tasks[sel].a0 = (reg_t) param;

        task_stat[sel].flag = 1;
        task_stat[sel].priority = priority;
        task_stat[sel].timeslice = timeslice;
        task_stat[sel].timelast = timeslice;
        _tot++;
		return 0;
	} else {
		return -1;
	}
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
	/* trigger a machine-level software interrupt */
	int id = r_mhartid();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}


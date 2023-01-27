#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);


#define MAX_TASKS 10
#define STACK_SIZE 1024

#define TASK_ALIVE (1 << 0)

struct context os_shed;
uint8_t os_shed_stack[STACK_SIZE];

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct sched_flag {
    uint8_t flag;
    uint8_t priority;
};
struct sched_flag task_stat[MAX_TASKS];
uint8_t rank[MAX_TASKS];
struct context ctx_tasks[MAX_TASKS];
/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _tot = 0;
static int _select = -1;
static int _current = -1;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}
int calculate_prioriry(int task_no) {
    return task_stat[task_no].priority;
}
void swap(uint8_t *a, uint8_t *b) {
    uint8_t c = *b;
    *b = *a;
    *a = c;
}
void arrange_rank() {
    int idx = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_stat[i].flag & TASK_ALIVE) {
            rank[idx++] = i;
        }
    }
    for (int i = 0; i < idx; i++) {
        for (int j = idx - 1; j > i; j--) {
            if (calculate_prioriry(rank[j]) < calculate_prioriry(rank[j - 1])) {
                swap(&rank[j], &rank[j - 1]);
            }
        }
    }
}

void os_swich_task() {
	schedule();
}

void sched_init()
{
    os_shed.sp = (reg_t)&os_shed_stack[STACK_SIZE - 1];
    os_shed.ra = (reg_t)os_swich_task;

    for (int i = 0; i < MAX_TASKS; i++) {
        task_stat[i].flag = 0;
        task_stat[i].priority = -1;
    }
	w_mscratch(0);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	if (_tot <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}

	_select = (_select + 1) % _tot;
    _current = rank[_select];
	struct context *next = &(ctx_tasks[_current]);
	switch_to(next);

}
int new_task_no() {
    int ret = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!(task_stat[i].flag & TASK_ALIVE)) {
            ret = i;
            break;
        }
    }
    return ret;
}
/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routune entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*task)(void *param), void *param, uint8_t priority)
{
    int no = new_task_no();
    if (no == -1) {
        return -1;
    }
    _tot += 1;

    ctx_tasks[no].sp = (reg_t) &task_stack[no][STACK_SIZE - 1];
    ctx_tasks[no].ra = (reg_t) task;
    ctx_tasks[no].a0 = (reg_t) param;
    task_stat[no].priority = priority;
    task_stat[no].flag = TASK_ALIVE;

    arrange_rank();
	return 0;
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
    switch_to(&os_shed);
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}
void task_delete(uint8_t task) {
    task_stat[task].flag = 0;
    task_stat[task].priority = -1;
}

void task_exit() {
    _tot -= 1;

    task_delete(_current); 
    
    arrange_rank();

    schedule();
}
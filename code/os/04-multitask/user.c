#include "os.h"

#define DELAY 1000

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");
	while (1) {
		uart_puts("Task 0: Running...\n");
		task_delay(DELAY);
		task_yield();
	}
}

void user_task1(void)
{
	uart_puts("Task 1: Created!\n");
    int cnt = 0;
	while (1) {
		uart_puts("Task 1: Running...\n");
		task_delay(DELAY);
		task_yield();
        cnt++;
        if (cnt == 1) task_exit();
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task0, NULL, 1);
	task_create(user_task1, NULL, 0);
}


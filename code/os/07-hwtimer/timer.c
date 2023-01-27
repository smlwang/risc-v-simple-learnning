#include "os.h"

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
	/* each CPU has a separate source of timer interrupts. */
	int id = r_mhartid();
	
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{
	/*
	 * On reset, mtime is cleared to zero, but the mtimecmp registers 
	 * are not reset. So we have to init the mtimecmp manually.
	 */
	timer_load(TIMER_INTERVAL);

	/* enable machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

	/* enable machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
}
char time[] = "000000";
void add(int i) {
    if (time[i] > '9') {
        time[i] = '0'; 
        time[i - 1] += 1;
    }
    if (i > 1 && time[i - 1] > '5') {
        time[i - 1] = '0';
        time[i - 2] += 1;
    }
}
void timer_handler() 
{
	_tick++;
	// printf("tick: %d\n", _tick);
    time[5] += 1;
    add(5);
    add(3);
    add(1);
    printf("%c%c:%c%c:%c%c\r", time[0], time[1], time[2], time[3], time[4], time[5]);
	timer_load(TIMER_INTERVAL);
}

#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);

void malloc_test()
{
    void *p1 = malloc(1024);
    void *p2 = malloc(1024);
    void *p3 = malloc(2048);
    // printf("p1 = 0x%x\n", p1);
    // printf("p2 = 0x%x\n", p2);
    // printf("p3 = 0x%x\n", p3);
    // free(p2);
    // p2 = malloc(256);
    // free(p1);
    // free(p2);
    // free(p3);

    printf("p1 = 0x%x\n", p1);
    printf("p2 = 0x%x\n", p2);
    printf("p3 = 0x%x\n", p3);

}
void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");

	page_init();
    malloc_test();
	while (1) {}; // stop here!
}


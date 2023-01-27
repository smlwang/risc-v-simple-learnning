extern void uart_init(void);
extern void uart_puts(char *s);
extern char uart_readc();
extern void uart_putc();
void echo(void) {
    char c = uart_readc();
    if (c == '\r') {
        c = '\n';
    }
    uart_putc(c);
}
void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");

	while (1) {
        echo();
    } // stop here!
}


#include "platform.h"

	# size of each hart's stack is 1024 bytes
	.equ	STACK_SIZE, 1024

	.global	_start

	.text
_start:
	# park harts with id != 0
	csrr	t0, mhartid		# read current hart id
	mv	tp, t0			# keep CPU's hartid in its tp for later usage.
	bnez	t0, park		# if we're not on the hart 0
					# we park the hart
	# Setup stacks, the stack grows from bottom to top, so we put the
	# stack pointer to the very end of the stack range.
	slli	t0, t0, 10		# shift left the hart id by 1024
	la	sp, stacks + STACK_SIZE	# set the initial stack pointer
					# to the end of the first stack space
	add	sp, sp, t0		# move the current hart stack pointer
					# to its place in the stack space

	j	start_kernel		# hart 0 jump to c

park:
	wfi
	j	park

start_kernel:
    call uart_init
    la a0, hello_str
    call uart_puts
    
stop:
    j stop

uart_write:
    li t0, UART0
    add t0, t0, a0
    sb a1, 0(t0)
    ret

uart_read:
    li t0, UART0
    add t0, t0, a0
    lb a0, 0(t0)
    ret

uart_init:
    addi sp, sp, -4
    sw ra, 0(sp)

    li a0, 1
    li a1, 0
    call uart_write

    li a0, 3
    call uart_read
    ori a0, a0, 0x80
    mv a1, a0
    li a0, 3
    call uart_write

    li a0, 0
    li a1, 0x03
    call uart_write

    li a0, 1
    li a1, 0x00
    call uart_write

    li a1, 0
    ori a1, a1, 3
    li a0, 3
    call uart_write

    lw ra, 0(sp)
    addi sp, sp, 4    
    ret

uart_putc:
    addi sp, sp, -8
    sw s0, 0(sp)
    sw ra, 4(sp)

    mv s0, a0

_putc_while_loop:
    li a0, 5
    call uart_read
    andi a0, a0, 0x20
    beqz a0, _putc_while_loop

    li a0, 0
    mv a1, s0 # a1 = s0
    call uart_write

    lw s0, 0(sp)
    lw ra, 4(sp)
    addi sp, sp, 8
    ret

uart_puts:
    addi sp, sp, -12 # sp = sp - 12
    sw ra, 0(sp)
    sw a0, 4(sp)
    sw s0, 8(sp)

    lw s0, 4(sp)
_puts_while_loop:
    lb a0, 0(s0)
    beqz a0, _out_puts_loop

    call uart_putc
    addi s0, s0, 1

    j _puts_while_loop
_out_puts_loop:

    lw ra, 0(sp) 
    lw s0, 8(sp) 
    addi sp, sp, 12

    ret

hello_str:
    .string "Hello, RVOS!"

stacks:
	.skip	STACK_SIZE * MAXNUM_CPU # allocate space for all the harts stacks

	.end				# End of file


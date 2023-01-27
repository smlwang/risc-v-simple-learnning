# asm call C function `foo`

    .text
    .global _start
    .global foo

_start:
    la sp, stack_end

    li a0, 3
    li a1, 4
    call foo

_stop:
    j _stop

    
stack_start:
    .rept 32
    .word 0
    .endr

stack_end:

.end

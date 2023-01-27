.text
.global _start

.macro inc reg
    addi \reg, \reg, 1
.endm

_start:
    
    la x5, array
    li x6, 0
    li x7, 0

loop:
    lb x6, 0(x5)
    beqz x6, out_loop
    inc x5
    inc x7
    j loop
out_loop:

_stop:
    j _stop  # jump to _stop

array: 
.string "hello, world!"
    .end
    
    

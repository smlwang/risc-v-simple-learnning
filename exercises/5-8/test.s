# unsigned int square(unsigned int i) {
#     return i * i;
# }
# unsigned int sum_squares(unsigned int i) {
#     unsigned int sum = 0;
#     for (int j = 1; j <= i; j++) {
#         sum += square(j);
#     }
#     return sum;
# }
# void _start() {
#     sum_squares(3);
# }
# 

    .text
    .global _start

_start:
    la sp, stack_end
    li a0, 3
    call sum_squares

stop:
    j stop  # jump to stop


square:
    mul a0, a0, a0
    ret

sum_squares
    addi sp, sp, -12; # sp = sp + -12
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw ra, 8(sp)

    # loop times
    mv s0, a0 # t0 = a0
    mv s1, zero # result

sum_loop:    
    ble s0, zero, out_loop # if s0 <= zero then target
    mv a0, s0
    call square
    add s1, s1, a0
    addi s0, s0, -1
    j sum_loop  # jump to sum_loop
    
out_loop:

    mv a0, s1    

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw ra, 8(sp)
    addi sp, sp, 12 # sp = sp + 12
    
    ret    

stack_start:
    .rept 4
    .word 0
    .endr   
stack_end:

    .end

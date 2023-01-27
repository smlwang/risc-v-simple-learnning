.text
.global _start
_start:
    li t1, 0x87654321
    srli t2, t1, 16
    slli t2, t2, 16
    sub t3, t1, t2
_stop:
    j _stop  # jump to _stop
    
    .end
    
    

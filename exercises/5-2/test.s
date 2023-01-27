.text
.global _start
_start:
#  a, b, c, d, e
#t 1, 2, 3, 4, 5
    li t2, 1 # b = 1
    li t3, 2 # c = 2
    li t5, 3 # e = 3
    add t1, t2, t3 # a = b + c
    sub t4, t1, t5 # d = a - e

_stop:
    j _stop  # jump to _stop
    
    .end
    
    

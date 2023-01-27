.text
.global _start
_start:
    la x5, _array
    lw x6, 0(x5)
    lw x7, 4(x5)


_stop:
    j _stop  # jump to _stop
    
    
_array:
    .word 0x11111111
    .word 0xffffffff

    .end

.text
.global _start

.macro set_struct struct
    la x5, \struct
    sw x6, 0(x5)
    sw x7, 4(x5)
.endm

.macro get_struct struct
    la x5, \struct
    lw x6, 4(x5)
    lw x7, 0(x5)
.endm
_start:
    li x6, 0x12345678
    li x7, 0x87654321
    set_struct S
    li x6, 0
    li x7, 0
    get_struct S

_stop:
    j _stop  # jump to _stop
    
S:
    .word 0, 0
.end

    

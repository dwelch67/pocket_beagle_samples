
.globl _start
_start:
    ldr sp,=0x40300000
    bl notmain
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

.globl ASMDELAY
ASMDELAY:
    subs r0,r0,#1
    bne ASMDELAY
    bx lr


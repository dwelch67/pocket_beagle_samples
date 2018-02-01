
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

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr


.globl BRANCHTO
BRANCHTO:
    bx r0

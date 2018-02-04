
.globl _start
_start:
    ldr sp,=0x40300000
    bl start_l1cache
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

.globl start_l1cache
start_l1cache:
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 6 ;@ BPIALL Branch predictor invalidate all
    mcr p15, 0, r0, c7, c5, 0 ;@ ICIALLU Instruction cache invalidate all
    mcr p15, 0, r0, c8, c5, 0 ;@ ITLBIALL Invalidate entire instruction TLB
    mrc p15, 0, r0, c1, c0, 0
    orr r0,r0,#0x1000 ;@ instruction
    orr r0,r0,#0x0800 ;@ branch prediction
    mcr p15, 0, r0, c1, c0, 0
    ISB
    bx lr

.globl stop_l1cache
stop_l1cache:
    mrc p15, 0, r0, c1, c0, 0
    bic r0,r0,#0x1000 ;@ instruction
    bic r0,r0,#0x0800 ;@ branch prediction
    mcr p15, 0, r0, c1, c0, 0
    ISB
    bx lr

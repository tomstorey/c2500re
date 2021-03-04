    .title "crt0.s for m68k bare metal"

    .extern main

    .global _start
    .global __DefaultInterrupt

    .section .text
    .align 2
_start:
    /* Startup delay */
    move.l  #1000, %d0
0:  subq.l  #1, %d0
    bgt     0b

    /* Configure VBR */
    lea     0x01000000, %a0
    movec   %a0, %vbr

    /* Setup bus error vector in SCR */
    andi.w  #0xFFFE, 0x02110000

    /* Configure ROM and RAM sizes in MCR */
    move.w  #0x0034, 0x02110002     /* 8MByte RAM, 4Mbit ROMs */
    
    jmp     main

/*
 * __DefaultInterrupt handles all interrupt and exception vectors that have not
 * been overridden by the programmer.
 *
 * Unless handled more specifically, all exceptions and interrupts simply
 * return.
 */
__DefaultInterrupt:
    rte

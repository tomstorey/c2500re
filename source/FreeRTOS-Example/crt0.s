    .title "crt0.s for m68k bare metal"

    .extern main
    .extern _data_start
    .extern _data_end
    .extern _bss_start
    .extern _bss_end

    .global _start
    .global __DefaultInterrupt

    .section .text
    .align 2
_start:
    /* Startup delay */
    move.l  #1000, %d0
0:  subq.l  #1, %d0
    bgt     0b
    
    /* Reconfigure the VBR */
    lea     __rom_base, %a0
    movec   %a0, %vbr

    /* Clear BEV bit in SCR */
    andi.w  #0xFFFE, 0x02110000    

    /* Configure ROM and RAM sizes in MCR */
    move.w  #0x0034, 0x02110002     /* 8MByte RAM, 4Mbit ROMs */

    /* Enable access to peripherals */
    ori.w   #0x3F, 0x02110004
    nop
    nop
    nop
    nop
    andi.w  #0xFFC0, 0x02110004

.bss_init:
    /*
     * Clear the BSS area
     */
    movea.l #_bss_start, %a0
    movea.l #_bss_end, %a1
    move.q  #0, %d0

0:
    cmpa.l %a1, %a0
    bge    .data_copy

    move.l %d0, (%a0)+

    bra    0b

.data_copy:
    /*
     * Copy initialised data from ROM to RAM.
     */
    movea.l #_rodata_end, %a0   /* A0 is source pointer */
    movea.l #_data_start, %a1   /* A1 is destination pointer */
    movea.l #_data_end, %a2     /* A2 is end of .data */

0:
    /*
     * Copy long words, post incrementing pointers, until the destination
     * pointer equals the end of the .data section.
     */
    cmpa.l  %a1, %a2            /* If no data to copy, skip */
    beq     .call_main

    move.l  (%a0)+, (%a1)+
    bra     0b

.call_main:
    /* Start application */
    jmp     main

    /* There is no return from main, so ... undefined behaviour from here. */

/*
 * __DefaultInterrupt handles all interrupt and exception vectors that have not
 * been overridden by the programmer.
 *
 * Unless handled more specifically, all exceptions and interrupts simply
 * return.
 */
__DefaultInterrupt:
    rte

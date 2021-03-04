#ifndef C2500_H
#define C2500_H

#include <stdint.h>

#define SYSCON (*(volatile uint16_t *)(0x02110000))
typedef union {
    struct {
        uint16_t PTR:8;
        uint16_t :3;
        uint16_t LED:1;
        uint16_t PTRA:1;
        uint16_t PTEN:1;
        uint16_t :1;
        uint16_t BEV:1;
    };
    struct {
        uint16_t u16;
    };
} __SYSCONbits_t;
#define SYSCONbits (*(volatile __SYSCONbits_t *)(0x02110000))

#define MEMCON (*(volatile uint16_t *)(0x02110002))
typedef union {
    struct {
        uint16_t :10;
        uint16_t RAMSZ1:1;
        uint16_t RAMSZ0:1;
        uint16_t ROMSZ1:1;
        uint16_t ROMSZ0:1;
        uint16_t :2;
    };
    struct {
        uint16_t :10;
        uint16_t RAMSZ:2;
        uint16_t ROMSZ:2;
        uint16_t :2;
    };
    struct {
        uint16_t u16;
    };
} __MEMCONbits_t;
#define MEMCONbits (*(volatile __MEMCONbits_t *)(0x02110002))

#define PERIPHACC (*(volatile uint16_t *)(0x02110004))
typedef union {
    struct {
        uint16_t :11;
        uint16_t EN5:1;
        uint16_t EN4:1;
        uint16_t EN3:1;
        uint16_t EN2:1;
        uint16_t EN1:1;
        uint16_t EN0:1;
    };
    struct {
        uint16_t :11;
        uint16_t EN:6;
    };
    struct {
        uint16_t u16;
    };
} __PERIPHACCbits_t;
#define PERIPHACCbits (*(volatile __PERIPHACCbits_t *)(0x02110004))

#define INTSRC (*(volatile uint16_t *)(0x02110006))
typedef union {
    struct {
        uint16_t :13;
        uint16_t SER:1;
        uint16_t ETH:1;
        uint16_t :1;
    };
    struct {
        uint16_t u16;
    };
} __INTSRCbits_t;
#define INTSRCbits (*(volatile __INTSRCbits_t *)(0x02110006))

#define FLASHPD (*(volatile uint16_t *)(0x0211000A))
typedef union {
    struct {
        uint16_t :3;
        uint16_t C0PD:1;
        uint16_t C1PD:1;
        uint16_t :1;
        uint16_t C0PD5:1;
        uint16_t C0PD2:1;
        uint16_t :8;
    };
    struct {
        uint16_t u16;
    };
} __FLASHPDbits_t;
#define FLASHPDbits (*(volatile __FLASHPDbits_t *)(0x0211000A))

#define NOVRAM (*(volatile uint8_t *)(0x02110060))
typedef union {
    struct {
        uint8_t :4;
        uint8_t CE:1;
        uint8_t SK:1;
        uint8_t DO:1;
        uint8_t DI:1;
    };
    struct {
        uint8_t u8;
    };
} __NOVRAMbits_t;
#define NOVRAMbits (*(volatile __NOVRAMbits_t *)(0x02110060))

#define WDTCON (*(volatile uint8_t *)(0x02120040))
typedef union {
    struct {
        uint8_t :1;
        uint8_t WDTEN:1;
        uint8_t :6;
    };
    struct {
        uint8_t u8;
    };
} __WDTCONbits_t;
#define WDTCONbits (*(volatile __WDTCONbits_t *)(0x02120040))

#define WDTO (*(volatile uint16_t *)(0x02120050))
#define TIMERPS (*(volatile uint16_t *)(0x02120070))

#endif /* C2500_H */

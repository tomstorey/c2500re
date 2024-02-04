#ifndef SCN2681_H
#define	SCN2681_H

#include <stdint.h>

#define MR1A (*(volatile uint8_t *)(0x02120100))
#define MR1B (*(volatile uint8_t *)(0x02120108))
typedef union {
    struct {
        uint8_t RXRTS:1;
        uint8_t RXINT:1;
        uint8_t ERROR:1;
        uint8_t PARITYMODE1:1;
        uint8_t PARITYMODE0:1;
        uint8_t PARITYTYPE:1;
        uint8_t BITS1:1;
        uint8_t BITS0:1;
    };
    struct {
            uint8_t :3;
            uint8_t PARITYMODE:2;
            uint8_t :1;
            uint8_t BITS:2;
    };
    struct {
        uint8_t u8;
    };
} __MR1bits_t;
#define MR1Abits (*(volatile __MR1bits_t *)(0x02120100))
#define MR1Bbits (*(volatile __MR1bits_t *)(0x02120108))

#define MR2A (*(volatile uint8_t *)(0x02120100))
#define MR2B (*(volatile uint8_t *)(0x02120108))
typedef union {
    struct {
        uint8_t CHMODE1:1;
        uint8_t CHMODE0:1;
        uint8_t TXRTS:1;
        uint8_t CTSEN:1;
        uint8_t STOPLEN3:1;
        uint8_t STOPLEN2:1;
        uint8_t STOPLEN1:1;
        uint8_t STOPLEN0:1;
    };
    struct {
            uint8_t CHMODE:2;
            uint8_t :2;
            uint8_t STOPLEN:4;
    };
    struct {
        uint8_t u8;
    };
} __MR2bits_t;
#define MR2Abits (*(volatile __MR2bits_t *)(0x02120100))
#define MR2Bbits (*(volatile __MR2bits_t *)(0x02120108))

#define SRA (*(volatile uint8_t *)(0x02120101))
#define SRB (*(volatile uint8_t *)(0x02120109))
typedef union {
    struct {
        uint8_t RXBRK:1;
        uint8_t FERR:1;
        uint8_t PERR:1;
        uint8_t OERR:1;
        uint8_t TXEMT:1;
        uint8_t TXRDY:1;
        uint8_t FFULL:1;
        uint8_t RXRDY:1;
    };
    struct {
        uint8_t u8;
    };
} __SRbits_t;
#define SRAbits (*(volatile __SRbits_t *)(0x02120101))
#define SRBbits (*(volatile __SRbits_t *)(0x02120109))

#define CSRA (*(volatile uint8_t *)(0x02120101))
#define CSRB (*(volatile uint8_t *)(0x02120109))
typedef union {
    struct {
        uint8_t RCS3:1;
        uint8_t RCS2:1;
        uint8_t RCS1:1;
        uint8_t RCS0:1;
        uint8_t TCS3:1;
        uint8_t TCS2:1;
        uint8_t TCS1:1;
        uint8_t TCS0:1;
    };
    struct {
        uint8_t RCS:4;
        uint8_t TCS:4;
    };
    struct {
        uint8_t u8;
    };
} __CSRbits_t;
#define CSRAbits (*(volatile __CSRbits_t *)(0x02120101))
#define CSRBbits (*(volatile __CSRbits_t *)(0x02120109))

#define CRA (*(volatile uint8_t *)(0x02120102))
#define CRB (*(volatile uint8_t *)(0x0212010A))
typedef union {
    struct {
        uint8_t :1;
        uint8_t CMD2:1;
        uint8_t CMD1:1;
        uint8_t CMD0:1;
        uint8_t TXDIS:1;
        uint8_t TXEN:1;
        uint8_t RXDIS:1;
        uint8_t RXEN:1;
    };
    struct {
        uint8_t :1;
        uint8_t CMD:3;
        uint8_t :4;
    };
    struct {
        uint8_t u8;
    };
} __CRbits_t;
#define CRAbits (*(volatile __CRbits_t *)(0x02120102))
#define CRBbits (*(volatile __CRbits_t *)(0x0212010A))

#define HRA (*(volatile uint8_t *)(0x02120103))
#define HRB (*(volatile uint8_t *)(0x0212010B))

#define IPCR (*(volatile uint8_t *)(0x02120104))
typedef union {
    struct {
        uint8_t IP3DLT:1;
        uint8_t IP2DLT:1;
        uint8_t IP1DLT:1;
        uint8_t IP0DLT:1;
        uint8_t IP3:1;
        uint8_t IP2:1;
        uint8_t IP1:1;
        uint8_t IP0:1;
    };
    struct {
        uint8_t u8;
    };
} __IPCRbits_t;
#define IPCRbits (*(volatile __IPCRbits_t *)(0x02120104))

#define ACR (*(volatile uint8_t *)(0x02120104))
typedef union {
    struct {
        uint8_t BRGSET:1;
        uint8_t SRC2:1;
        uint8_t SRC1:1;
        uint8_t SRC0:1;
        uint8_t IP3DLTIE:1;
        uint8_t IP2DLTIE:1;
        uint8_t IP1DLTIE:1;
        uint8_t IP0DLTIE:1;
    };
    struct {
        uint8_t :1;
        uint8_t SRC:3;
        uint8_t :4;
    };
    struct {
        uint8_t u8;
    };
} __ACRbits_t;
#define ACRbits (*(volatile __ACRbits_t *)(0x02120104))

#define ISR (*(volatile uint8_t *)(0x02120105))
#define IMR (*(volatile uint8_t *)(0x02120105))
typedef union {
    struct {
        uint8_t IPC:1;
        uint8_t BRKBDLT:1;
        uint8_t RXRDYB:1;
        uint8_t TXRDYB:1;
        uint8_t CTRRDY:1;
        uint8_t BRKADLT:1;
        uint8_t RXRDYA:1;
        uint8_t TXRDYA:1;
    };
    struct {
        uint8_t :2;
        uint8_t FFULLB:1;
        uint8_t :3;
        uint8_t FFULLA:1;
        uint8_t :1;
    };
    struct {
        uint8_t u8;
    };
} __ISRbits_t;
#define ISRbits (*(volatile __ISRbits_t *)(0x02120105))
#define IMRbits (*(volatile __IMRbits_t *)(0x02120105))

#define CTU (*(volatile uint8_t *)(0x02120106))
#define CTL (*(volatile uint8_t *)(0x02120107))
#define CTUR (*(volatile uint8_t *)(0x02120106))
#define CTLR (*(volatile uint8_t *)(0x02120107))

#define INPORT (*(volatile uint8_t *)(0x0212010D))
typedef union {
    struct {
        uint8_t :1;
        uint8_t IP6:1;
        uint8_t IP5:1;
        uint8_t IP4:1;
        uint8_t IP3:1;
        uint8_t IP2:1;
        uint8_t IP1:1;
        uint8_t IP0:1;
    };
    struct {
        uint8_t u8;
    };
} __INPORTbits_t;
#define INPORTbits (*(volatile __INPORTbits_t *)(0x0212010D))

#define OPCR (*(volatile uint8_t *)(0x0212010D))
typedef union {
    struct {
        uint8_t OP7CFG:1;
        uint8_t OP6CFG:1;
        uint8_t OP5CFG:1;
        uint8_t OP4CFG:1;
        uint8_t OP3CFG1:1;
        uint8_t OP3CFG0:1;
        uint8_t OP2CFG1:1;
        uint8_t OP2CFG0:1;
    };
    struct {
        uint8_t :4;
        uint8_t OP3CFG:2;
        uint8_t OP2CFG:2;
    };
    struct {
        uint8_t u8;
    };
} __OPCRbits_t;
#define OPCRbits (*(volatile __OPCRbits_t *)(0x0212010D))

#define OUTPORTSET (*(volatile uint8_t *)(0x0212010E))
#define OUTPORTCLR (*(volatile uint8_t *)(0x0212010F))
typedef union {
    struct {
        uint8_t OP7:1;
        uint8_t OP6:1;
        uint8_t OP5:1;
        uint8_t OP4:1;
        uint8_t OP3:1;
        uint8_t OP2:1;
        uint8_t OP1:1;
        uint8_t OP0:1;
    };
    struct {
        uint8_t u8;
    };
} __OUTPORTATOMICbits_t;
#define OUTPORTSETbits (*(volatile __OUTPORTATOMICbits_t *)(0x0212010E))
#define OUTPORTCLRbits (*(volatile __OUTPORTATOMICbits_t *)(0x0212010F))

#define STARTCTR (*(volatile uint8_t *)(0x0212010E))
#define STOPCTR (*(volatile uint8_t *)(0x0212010F))

#endif	/* SCN2681_H */

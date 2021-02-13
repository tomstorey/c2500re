#include <stdint.h>

#define IER2 (*(volatile uint8_t *)(0x02132017))
typedef union {
    struct {
        uint8_t T3IRQE:1;
        uint8_t T2IRQE:1;
        uint8_t T1IRQE:1;
        uint8_t T0IRQE:1;
        uint8_t :4;
    };
    struct {
        uint8_t u8;
    };
} __IER2bits_t;
#define IER2bits (*(volatile __IER2bits_t *)(0x02132017))

#define ITCR (*(volatile uint8_t *)(0x02132019))
typedef union {
    struct {
        uint8_t IPC:1;
        uint8_t IAK1:1;
        uint8_t IAK0:1;
        uint8_t VOS:1;
        uint8_t :4;
    };
    struct {
        uint8_t :1;
        uint8_t IAK:2;
        uint8_t :5;
    };
    struct {
        uint8_t u8;
    };
} __ITCRbits_t;
#define ITCRbits (*(volatile __ITCRbits_t *)(0x02132019))

#define IVR (*(volatile uint8_t *)(0x0213201B))
#define IMVR (*(volatile uint8_t *)(0x0213201D))

#define TCNT0 (*(volatile uint16_t *)(0x02132060))
#define TCNT1 (*(volatile uint16_t *)(0x02132068))
#define TCNT2 (*(volatile uint16_t *)(0x02132070))
#define TCNT3 (*(volatile uint16_t *)(0x02132078))

#define TCONR0 (*(volatile uint16_t *)(0x02132062))
#define TCONRH0 (*(volatile uint8_t *)(0x02132062))
#define TCONRL0 (*(volatile uint8_t *)(0x02132063))
#define TCONR1 (*(volatile uint16_t *)(0x0213206A))
#define TCONR2 (*(volatile uint16_t *)(0x02132072))
#define TCONR3 (*(volatile uint16_t *)(0x0213207A))

#define TCSR0 (*(volatile uint8_t *)(0x02132065))
#define TCSR1 (*(volatile uint8_t *)(0x0213206D))
#define TCSR2 (*(volatile uint8_t *)(0x02132075))
#define TCSR3 (*(volatile uint8_t *)(0x0213207D))

typedef union {
    struct {
        uint8_t CMF:1;
        uint8_t ECMI:1;
        uint8_t :1;
        uint8_t TME:1;
        uint8_t :4;
    };
    struct {
        uint8_t u8;
    };
} __TCSRbits_t;
#define TCSR0bits (*(volatile __TCSRbits_t *)(0x02132065))
#define TCSR1bits (*(volatile __TCSRbits_t *)(0x0213206D))
#define TCSR2bits (*(volatile __TCSRbits_t *)(0x02132075))
#define TCSR3bits (*(volatile __TCSRbits_t *)(0x0213207D))

#define TEPR0 (*(volatile uint8_t *)(0x02132064))
#define TEPR1 (*(volatile uint8_t *)(0x0213206C))
#define TEPR2 (*(volatile uint8_t *)(0x02132074))
#define TEPR3 (*(volatile uint8_t *)(0x0213207C))

typedef union {
    struct {
        uint8_t :5;
        uint8_t ECKS2:1;
        uint8_t ECKS1:1;
        uint8_t ECKS0:1;
    };
    struct {
        uint8_t :5;
        uint8_t ECKS:3;
    };
    struct {
        uint8_t u8;
    };
} __TEPRbits_t;
#define TEPR0bits (*(volatile __TEPRbits_t *)(0x02132064))
#define TEPR1bits (*(volatile __TEPRbits_t *)(0x0213206C))
#define TEPR2bits (*(volatile __TEPRbits_t *)(0x02132074))
#define TEPR3bits (*(volatile __TEPRbits_t *)(0x0213207C))

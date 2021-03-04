#include <stdint.h>
#include "C2500.h"
#include "SCN2681.h"

#define STATE_DEFAULT 0
#define STATE_PING 1
#define STATE_LOAD_CODE_GET_LENGTH 2
#define STATE_LOAD_CODE_GET_ADDR 3
#define STATE_LOAD_CODE_GET_DATA 4
#define STATE_EXECUTE 5
#define STATE_JUMP 6

#define COMMAND_NONE 0
#define COMMAND_RX_PING 1
#define COMMAND_TX_PONG 2
#define COMMAND_RX_LOAD_CODE 3
#define COMMAND_TX_CODE_LOADED 4
#define COMMAND_RX_EXECUTE 5
#define COMMAND_RX_JUMP 6
#define COMMAND_TX_RUNNING 7

void configure_uart(void);
uint8_t uart_get_char(void);
void uart_send_char(uint8_t data);

int
main(void)
{
    configure_uart();

    uint8_t state = STATE_DEFAULT;      /* Current state */
    uint8_t command = COMMAND_NONE;     /* Current command */

    uint32_t code_len = 0;
    uint32_t addr = 0;
    uint8_t *code_ptr;
    uint32_t ctr = 0;

    /* Infinite loop for the state machine */
    for (;;) {
        switch (state) {
            case STATE_PING:
                /* The loader script can ping the bootloader to see when it is
                 * ready to accept data */
                uart_send_char((uint8_t)COMMAND_TX_PONG);

                state = STATE_DEFAULT;

                break;

            case STATE_LOAD_CODE_GET_LENGTH:
                /* When loading code, the first thing to do is receive the
                 * length of the incomming data. The first 4 bytes to be
                 * received are the length */
                code_len |= uart_get_char();
                ctr++;

                if (ctr == 4) {
                    state = STATE_LOAD_CODE_GET_ADDR;

                    ctr = 0;
                } else {
                    code_len <<= 8;
                }

                break;

            case STATE_LOAD_CODE_GET_ADDR:
                /* The next 4 bytes to be received will be the address where
                 * the received data should be loaded */
                addr |= uart_get_char();
                ctr++;

                if (ctr == 4) {
                    state = STATE_LOAD_CODE_GET_DATA;

                    ctr = 0;
                    code_ptr = (uint8_t *)addr;
                } else {
                    addr <<= 8;
                }

                break;

            case STATE_LOAD_CODE_GET_DATA:
                /* Now receive further bytes until code_len is decremented to
                 * zero */
                *code_ptr = uart_get_char();
                code_ptr++;
                code_len--;

                if (code_len == 0) {
                    uart_send_char((uint8_t)COMMAND_TX_CODE_LOADED);

                    state = STATE_DEFAULT;
                }

                break;

            case STATE_EXECUTE:
            case STATE_JUMP:
                /* When executing, receive 4 bytes that will form the address
                 * to jump to, then JSR or JMP to that address */
                addr |= uart_get_char();
                ctr++;

                if (ctr == 4 && state == STATE_EXECUTE) {
                    uart_send_char((uint8_t)COMMAND_TX_RUNNING);

                    if (state == STATE_EXECUTE) {
                        asm volatile(
                            /* Put addr into A0 then jump to subroutine  */
                            "movea.l    %[addr], %%a0                   \n\t"
                            "jsr        %%a0@                           \n\t"

                            /* If the user code should happen to return, and if
                             * it should happen to work successfully, go back
                             * to default state */
                            :
                            :[addr]"rm"(addr)
                            :
                        );

                        state = STATE_DEFAULT;
                    } else {
                        asm volatile(
                            /* Put addr into A0 then jump  */
                            "movea.l    %[addr], %%a0                   \n\t"
                            "jmp        %%a0@                           \n\t"

                            /* User code cannot return from a jump */
                            :
                            :[addr]"rm"(addr)
                            :
                        );
                    }
                } else {
                    addr <<= 8;
                }

                break;

            case STATE_DEFAULT:
            default:
                /* Set the OK LED to indicate waiting for a command */
                SYSCONbits.LED = 1;

                /* In the default state we wait for a character to be received
                 * which indicates the command to be executed */
                command = uart_get_char();

                /* Set the next state machine state based on the command */
                switch (command) {
                    case COMMAND_RX_PING:
                        /* Respond to ping */
                        state = STATE_PING;

                        break;

                    case COMMAND_RX_LOAD_CODE:
                        /* Loading code */
                        state = STATE_LOAD_CODE_GET_LENGTH;

                        break;

                    case COMMAND_RX_EXECUTE:
                        /* Execute (JSR) to an address */
                        state = STATE_EXECUTE;

                        break;

                    case COMMAND_RX_JUMP:
                        /* Execute (JMP) to an address */
                        state = STATE_JUMP;

                        break;

                    default:
                        /* Invalid command */
                        command = COMMAND_NONE;

                        break;
                }

                if (command != COMMAND_NONE) {
                    ctr = 0;

                    /* Clear the OK LED */
                    SYSCONbits.LED = 0;
                }

                break;
        }
    }
}

void
configure_uart(void)
{
    IMR = 0;        /* Disable all interrupts */
    CRA = 0x10;     /* Reset command pointer */
    MR1A = 0x13;    /* No parity, 8 bits per char */
    MR2A = 0x7;     /* 1 stop bit */
    ACR = 0xE0;     /* BRG set 2, external clock */
    CSRA = 0xDD;    /* Timer for baud generation */
    CTUR = 0;       /* Timer value = 0x0002 */
    CTLR = 0x2;
    (void)STARTC;   /* Start timer */
    CRA = 0x5;      /* Enable transmitter and receiver */
    OPCR = 0x4;     /* Timer to OP3 */
    SOPB = 0x1;
}

uint8_t
uart_get_char(void)
{
    /*
     * Waits for the RxRDY flag to be set in the SRA register, and then
     * returns the contents of the receive holding register
     */
    while ((SRA & 0x1) == 0) {
        /* Wait for a character to be received */
    }

    return RHRA;
}

void
uart_send_char(uint8_t data) {
    /*
     * Waits for the TxRDY flag to be set in the SRA register, and then places
     * the contents of data into the transmit holding register
     */
    while ((SRA & 0x4) == 0) {
        /* Wait for transmitter to be available */
    }

    THRA = data;
}

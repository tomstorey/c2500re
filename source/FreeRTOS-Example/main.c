#include <stdint.h>
#include <stdbool.h>

#include "C2500.h"
#include "HD64570.h"

#include "FreeRTOS-Kernel/include/FreeRTOS.h"
#include "FreeRTOS-Kernel/include/task.h"
#include "FreeRTOS-Kernel/include/timers.h"

static TaskHandle_t task_handle = NULL;

static void task(void *param);

void __attribute__((interrupt))
IRQ4(void)
{
    /*
     * ISR for IRQ4 - tick timer
     */ 
    __INTSRCbits_t intsrc;
    __ISR2bits_t isr2;
    intsrc.u16 = INTSRC;

    if (intsrc.SER == 1) {
        isr2.u8 = ISR2;

        if (isr2.T0IRQ == 1) {
            /* Clear interrupt from HD64570 timer channel */
            (void)TCSR0;
            (void)TCNT0;

            /* Tick */
            asm volatile("trap #14");
        }
    }
}

void
vApplicationSetupInterrupts( void )
{
    /* The system clock (10MHz) is divided by 8 to derive the timer
     * base clock (BC) of 1.25MHz. */
    TCONR0 = (1250000 / configTICK_RATE_HZ);
    IER2bits.T0IRQE = 1;
    TCSR0bits.ECMI = 1;
    TCSR0bits.TME = 1;

    portENABLE_INTERRUPTS();
}

int
main(void)
{
    /* Create a task */
    xTaskCreate(task, "Task", 256, NULL, 3, &task_handle);

    vTaskStartScheduler();

    return 0;
}

static void
task(void *param)
{
    while (true) {
        /* Blink the OK LED at 1hz */
        SYSCONbits.LED ^= 1;

        vTaskDelay(50);
    }
}

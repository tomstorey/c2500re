#include <stdint.h>
#include <stdbool.h>

#include "HD64570.h"

#include "FreeRTOS-Kernel/include/FreeRTOS.h"
#include "FreeRTOS-Kernel/include/task.h"
#include "FreeRTOS-Kernel/include/timers.h"

#define SCR (*(volatile uint16_t *)(0x02110000))
#define ISR (*(volatile uint16_t *)(0x02110006))

static TaskHandle_t task_handle = NULL;

static void task(void *param);

void __attribute__((interrupt))
IRQ4(void)
{
    /*
     * ISR for IRQ4 - tick timer
     */
    uint16_t source = ISR;

    if ((source & 0x4) != 0) {
        /* Clear interrupt from HD64570 timer channel */
        (void)TCSR0;
        (void)TCNT0;

        /* Tick */
        asm volatile("trap #14");
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
        SCR ^= 0x10;

        vTaskDelay(50);
    }
}

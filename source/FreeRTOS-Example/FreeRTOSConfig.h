#ifndef FREERTOSCONFIG_H
#define	FREERTOSCONFIG_H

#define configUSE_PREEMPTION 1
#define configTICK_RATE_HZ ((TickType_t)100)
#define configMAX_PRIORITIES 4
#define configMINIMAL_STACK_SIZE 512
#define configMAX_TASK_NAME_LEN 32
#define configUSE_16_BIT_TICKS 0
#define configUSE_TASK_NOTIFICATIONS 1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 4

/* CPUs from 68010 and above will store a format word on the stack. Set to 1
 * to include this, or 0 if using a 68000 which does not use a format word,
 * when initialising a task stack. */
#define configM68K_INCLUDE_FORMAT_WORD 1

/* Memory allocation related definitions. */
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configTOTAL_HEAP_SIZE 20480
#define configISR_STACK_SIZE configMINIMAL_STACK_SIZE

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0

/* Software timer related definitions. */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY 1
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
#define configKERNEL_INTERRUPT_PRIORITY 0x4
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 0x7

/* Optional functions */
#define INCLUDE_vTaskDelay 1

#endif	/* FREERTOSCONFIG_H */

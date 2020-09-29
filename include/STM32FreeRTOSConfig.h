#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_CMSIS_RTOS_V2           0
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 extern uint32_t SystemCoreClock;
#endif
#if defined(configUSE_CMSIS_RTOS_V2) && (configUSE_CMSIS_RTOS_V2 == 1)
#define configSUPPORT_STATIC_ALLOCATION   1
#define configMAX_PRIORITIES              (56)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configMINIMAL_STACK_SIZE          ((uint16_t)128)
#define configTOTAL_HEAP_SIZE             ((size_t)(15 * 1024))
#else
extern char _end; /* Defined in the linker script */
extern char _estack; /* Defined in the linker script */
extern char _Min_Stack_Size; /* Defined in the linker script */
#define configMAX_PRIORITIES              (7)

#define configMINIMAL_STACK_SIZE          ((uint16_t)((uint32_t)&_Min_Stack_Size/8))
#define configTOTAL_HEAP_SIZE             ((size_t)(&_estack - _Min_Stack_Size - &_end))

#endif

#define configUSE_PREEMPTION              1
#define configUSE_IDLE_HOOK               1
#define configUSE_TICK_HOOK               1
#define configCPU_CLOCK_HZ                (SystemCoreClock)
#define configTICK_RATE_HZ                ((TickType_t)800)
#define configMAX_TASK_NAME_LEN           (16)
#define configUSE_TRACE_FACILITY          1
#define configUSE_16_BIT_TICKS            0
#define configIDLE_SHOULD_YIELD           1
#define configUSE_MUTEXES                 1
#define configQUEUE_REGISTRY_SIZE         8
#define configCHECK_FOR_STACK_OVERFLOW    0
#define configUSE_RECURSIVE_MUTEXES       1
#define configUSE_MALLOC_FAILED_HOOK      0
#define configUSE_APPLICATION_TASK_TAG    0
#define configUSE_COUNTING_SEMAPHORES     1
#define configGENERATE_RUN_TIME_STATS     0

#define configUSE_NEWLIB_REENTRANT        1

#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

#define configUSE_TIMERS             1
#define configTIMER_TASK_PRIORITY    (2)
#define configTIMER_QUEUE_LENGTH     10
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2)

#define INCLUDE_vTaskPrioritySet       1
#define INCLUDE_uxTaskPriorityGet      1
#define INCLUDE_vTaskDelete            1
#define INCLUDE_vTaskCleanUpResources  1
#define INCLUDE_vTaskSuspend           1
#define INCLUDE_vTaskDelayUntil        1
#define INCLUDE_vTaskDelay             1
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetIdleTaskHandle 1

#ifdef __NVIC_PRIO_BITS
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   0xf

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configKERNEL_INTERRUPT_PRIORITY   ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); } 

#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

#if defined(configUSE_CMSIS_RTOS_V2) && (configUSE_CMSIS_RTOS_V2 == 1)
#define xPortSysTickHandler osSystickHandler
#endif

#endif




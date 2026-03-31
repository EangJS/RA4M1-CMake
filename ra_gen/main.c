/* generated main source file - do not edit */
#include "hal_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SEGGER_RTT.h"
#include <stdio.h>

void ITM_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Unlock ITM
    ITM->LAR = 0xC5ACCE55;

    // Enable ITM
    ITM->TCR = ITM_TCR_ITMENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    ITM->TER = 1;

    TPIU->ACPR = 47U;   // (36 MHz / (47+1)) = 0.75 MHz
    TPIU->SPPR = 2;    // NRZ/Async mode
}

#define TEST_BLOCK_SIZE_MIN   32
#define TEST_BLOCK_SIZE_MAX   256
#define TEST_DELAY_MS         1000

static void Task1(void *pvParameters)
{
    (void) pvParameters;

    uint8_t *ptr;
    size_t blockSize;
    uint32_t counter = 0;

    for (;;)
    {
        /* Generate varying block size */
        blockSize = TEST_BLOCK_SIZE_MIN +
                    (counter % (TEST_BLOCK_SIZE_MAX - TEST_BLOCK_SIZE_MIN));

        SEGGER_RTT_printf(0, "\n[MallocTest] Requesting %u bytes\n", (unsigned int)blockSize);

        /* Allocate memory */
        ptr = (uint8_t *)pvPortMalloc(blockSize);

        if (ptr == NULL)
        {
            SEGGER_RTT_printf(0, "[MallocTest] Allocation FAILED!\n");
        }
        else
        {
            printf("[MallocTest] Allocation OK\n");

            /* Fill memory with pattern */
            memset(ptr, 0xA5, blockSize);

            /* Verify memory */
            for (size_t i = 0; i < blockSize; i++)
            {
                if (ptr[i] != 0xA5)
                {
                    SEGGER_RTT_printf(0, "[MallocTest] Memory corruption detected at %u!\n", (unsigned int)i);
                    break;
                }
            }

            /* Free memory */
            vPortFree(ptr);
            SEGGER_RTT_printf(0, "[MallocTest] Memory freed\n");
        }

        /* Optional heap stats */
        SEGGER_RTT_printf(0, "[MallocTest] Free heap: %u bytes\n",
                           (unsigned int)xPortGetFreeHeapSize());

        counter++;

        vTaskDelay(pdMS_TO_TICKS(TEST_DELAY_MS));
    }
}

// Task 2: Blinks "World" message
void Task2(void *pvParameters)
{
    while (1)
    {
        SEGGER_RTT_WriteString(0, "Task 2: World!\n");
        vTaskDelay(pdMS_TO_TICKS(1500)); // delay 1.5 seconds
    }
}

int main(void)
{
    ITM_Init();

    // Create two tasks
    xTaskCreate(Task1, "Task1", 256, NULL, 1, NULL);
    xTaskCreate(Task2, "Task2", 256, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1) {}
}

/* generated main source file - do not edit */
#include "hal_data.h"

void ITM_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Unlock ITM
    ITM->LAR = 0xC5ACCE55;

    // Enable ITM
    ITM->TCR = ITM_TCR_ITMENA_Msk;
    ITM->TER = 1;

    TPIU->ACPR = 47U;   // (36 MHz / (47+1)) = 0.75 MHz
    TPIU->SPPR = 2;    // NRZ/Async mode
}


int main(void)
{
    ITM_Init();
    hal_entry ();
    return 0;
}

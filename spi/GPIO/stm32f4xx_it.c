#include "stm32f4xx.h"
__IO uint32_t TimmingDelay;
void SysTick_Handler(void)
{
  if(TimmingDelay !=0)
  {
    TimmingDelay --;
   }
}        
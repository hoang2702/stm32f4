#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"

#define CS_ON()		GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define CS_OFF()	GPIO_SetBits(GPIOE, GPIO_Pin_3)

void SPI_ini(void);

#endif

#ifndef MAIN_H
#define MAIN_H

#include "mcu_ini.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "Flash.h"
#include "SPI.h"


#define	RED_ON()				GPIO_SetBits(GPIOD, GPIO_Pin_14)
#define	RED_OFF()				GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define	BLUE_ON()				GPIO_SetBits(GPIOD, GPIO_Pin_15)
#define	BLUE_OFF()			GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define	GREEN_ON()			GPIO_SetBits(GPIOD, GPIO_Pin_12)
#define	GREEN_OFF()			GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define	YELLOW_ON()			GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define	YELLOW_OFF()		GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define	DELAY						1500

#define MODE_RED				0
#define MODE_GREEN			1
#define MODE_BLUE				2
#define MODE_YELLOW			3

#define BUTTON_READ()			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)


#endif

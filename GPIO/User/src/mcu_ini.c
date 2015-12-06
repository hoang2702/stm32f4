#include "mcu_ini.h"

char buffer[]="I am DMA!";
//----------------------------------------------
//
//----------------------------------------------
void LEDs_ini(void)
{
	GPIO_InitTypeDef GPIO_Init_LED;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_Init_LED);
}

//--------------------------------------------------
void Button_ini(void)
{
	GPIO_InitTypeDef GPIO_Init_Button;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_Button.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Button.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_Button);
}

//--------------------------------------------------
void USART2_ini(void)
{
	GPIO_InitTypeDef GPIO_Init_USART;
	USART_InitTypeDef USART_InitUser;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_USART.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init_USART.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_USART.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_USART.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_Init_USART);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitUser.USART_BaudRate=9600;
	USART_InitUser.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitUser.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitUser.USART_Parity=USART_Parity_No;
	USART_InitUser.USART_StopBits=USART_StopBits_1;
	USART_InitUser.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART2, &USART_InitUser);
	
	NVIC_EnableIRQ(USART2_IRQn);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

//--------------------------------------------------
void DMA_ini(void)
{
	DMA_InitTypeDef DMA_ini_USART;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_ini_USART.DMA_Channel = DMA_Channel_4;
	DMA_ini_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	DMA_ini_USART.DMA_Memory0BaseAddr = (uint32_t)buffer;
	DMA_ini_USART.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_ini_USART.DMA_BufferSize = sizeof(buffer);
	DMA_ini_USART.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_ini_USART.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_ini_USART.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_ini_USART.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_ini_USART.DMA_Mode = DMA_Mode_Normal;
	DMA_ini_USART.DMA_Priority = DMA_Priority_Medium;
	DMA_ini_USART.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_ini_USART.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_ini_USART.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_ini_USART.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA1_Stream6, &DMA_ini_USART);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
}

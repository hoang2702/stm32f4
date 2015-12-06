#include <stm32f4xx.h>
#define CS_ON()  GPIO_SetBits(GPIOE, GPIO_Pin_3)
#define CS_OFF() GPIO_ResetBits(GPIOE, GPIO_Pin_3)
uint16_t SPIData;
void gpio_init(){

GPIO_InitTypeDef 	GPIO_Init_LED;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_Init_LED);
}
void spi_init(){
	GPIO_InitTypeDef 	GPIO_Init_LED;
		SPI_InitTypeDef		spi_init_user;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_LED);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE, &GPIO_Init_LED);
	CS_OFF();
	spi_init_user.SPI_Direction =SPI_Direction_2Lines_FullDuplex ;
	spi_init_user.SPI_Mode = SPI_Mode_Master ;
	spi_init_user.SPI_DataSize = SPI_DataSize_8b ;
	spi_init_user.SPI_CPOL =  SPI_CPOL_High ;
	spi_init_user.SPI_CPHA = SPI_CPHA_2Edge ;
	spi_init_user.SPI_NSS = SPI_NSS_Soft ;
	spi_init_user.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64 ;
	spi_init_user.SPI_FirstBit = SPI_FirstBit_MSB ;
	spi_init_user.SPI_CRCPolynomial = 7 ;
	SPI_Init(SPI1,&spi_init_user);
	SPI_Cmd(SPI1,ENABLE);
}
int main(){
spi_init();

		while(1){ 
			CS_ON();
	SPI_I2S_SendData(SPI1, 0x8F);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {}
	SPIData = SPI_I2S_ReceiveData(SPI1);	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {}	
	SPI_I2S_SendData(SPI1, 0x00);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {}
	SPIData = SPI_I2S_ReceiveData(SPI1);	
	CS_OFF(); 
			
	if (SPIData == 0x3B)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	} else if (SPIData == 0x3F)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	} 


}
	}
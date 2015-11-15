#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "misc.h"
#include "stm32f4xx_exti.h"

/*      MASTER              SLAVE
  PA4 --- CONTROL_SS    PB12 --- SPI2_SS
  PA5 --- SPI1_SCK      PB13 --- SPI2_SCK
  PA6 --- SPI1_MISO     PB14 --- SPI2_MISO
  PA7 --- SPI1_MOSI     PB15 --- SPI2_MOSI
*/
#define SS_DIS GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SS_EN  GPIO_SetBits(GPIOA, GPIO_Pin_4)

volatile uint8_t SPI_data_send,SPI_data_get;

GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef   SPI_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;

void SPI_Configuration(void);
void Delay(__IO uint32_t nCount);
void GPIOD_Init(void);
int main(void)
	{	
		GPIOD_Init();
  SPI_Configuration();
  while (1)
  {
    //Chuan bi data de gui (chi gui gi bat SS)
    SPI_I2S_SendData(SPI2,6);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)!=RESET); //Cho khi gui xong thi thoat vong lap
	SPI_data_send = SPI_I2S_ReceiveData(SPI1); //Nhan data
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	SS_EN; //Bat SS
    SPI_I2S_SendData(SPI1, 7); //Gui data tu Master
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=RESET);//Cho khi gui xong thi thoat vong lap
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    SS_DIS; //Tat co
    Delay(1000);
  }
}
void GPIOD_Init(void) {
    GPIO_InitTypeDef gpioInit;
    //dau tien cho phep xung clock cap toi GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    //khoi tao GPIOD
    gpioInit.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    gpioInit.GPIO_Mode=GPIO_Mode_OUT;
    gpioInit.GPIO_Speed=GPIO_Speed_100MHz;
    gpioInit.GPIO_OType=GPIO_OType_PP;
    gpioInit.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpioInit);
	}
void SPI_Configuration(void)
{
  /* SPI_MASTER configuration ------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* SPI_SLAVE configuration ------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2);   // only connect to
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);   // only connect to
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);   // only connect to
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);   // only connect to

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);

  /* Enable SPI_SLAVE */
  SPI_Cmd(SPI2, ENABLE);
  /* Enable SPI_MASTER */
  SPI_Cmd(SPI1, ENABLE);
}

void SPI2_IRQHandler(void)
{
  if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
  {
    SPI_data_get = SPI_I2S_ReceiveData(SPI2);//Nhan data tu mastter
    //SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_RXNE);
  }
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

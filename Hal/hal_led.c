#include "stm32F10x.h"
#include "hal_led.h"

static void hal_LedConfig(void);


void hal_LedInit(void)
{
	hal_LedConfig();
}
/////////////////////////


static void hal_LedConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,ENABLE); 						 
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED1_PORT,LED1_PIN);
}
////


void hal_LedTurn(void)
{
	GPIO_WriteBit(LED1_PORT,LED1_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN)));
	GPIO_WriteBit(LED2_PORT,LED2_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN)));
}
/////



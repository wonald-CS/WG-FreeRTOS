#include "hal_led.h"
#include "hal_usart.h"
#include "hal_init.h"



void Hal_Init(void)
{
	hal_LedInit();	
	Hal_Usart_Init();
}
